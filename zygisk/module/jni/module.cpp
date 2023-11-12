#include <string.h>
#include <android/log.h>

#include "zygisk.hpp"
#include "classes_dex.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "SNFix/JNI", __VA_ARGS__)

class SafetyNetFixModule : public zygisk::ModuleBase
{
public:
    void onLoad(zygisk::Api *api, JNIEnv *env) override
    {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(zygisk::AppSpecializeArgs *args) override
    {
        auto process = env->GetStringUTFChars(args->nice_name, nullptr);

        bool isGms = memcmp(process, "com.google.android.gms", 22) == 0;
        isGmsUnstable = memcmp(process, "com.google.android.gms.unstable", 31) == 0;

        env->ReleaseStringUTFChars(args->nice_name, process);

        if (isGms)
            api->setOption(zygisk::FORCE_DENYLIST_UNMOUNT);

        api->setOption(zygisk::DLCLOSE_MODULE_LIBRARY);
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs *args) override
    {
        if (!isGmsUnstable)
            return;
        injectPayload();
    }

    void preServerSpecialize(zygisk::ServerSpecializeArgs *args) override
    {
        api->setOption(zygisk::DLCLOSE_MODULE_LIBRARY);
    }

private:
    zygisk::Api *api = nullptr;
    JNIEnv *env = nullptr;
    bool isGmsUnstable = false;

    void injectPayload()
    {
        // First, get the system classloader
        LOGD("get system classloader");
        auto clClass = env->FindClass("java/lang/ClassLoader");
        auto getSystemClassLoader = env->GetStaticMethodID(clClass, "getSystemClassLoader",
                                                           "()Ljava/lang/ClassLoader;");
        auto systemClassLoader = env->CallStaticObjectMethod(clClass, getSystemClassLoader);

        // Assuming we have a valid mapped module, load it. This is similar to the approach used for
        // Dynamite modules in GmsCompat, except we can use InMemoryDexClassLoader directly instead of
        // tampering with DelegateLastClassLoader's DexPathList.
        LOGD("create buffer");
        auto buf = env->NewDirectByteBuffer(classes_dex, classes_dex_len);
        LOGD("create class loader");
        auto dexClClass = env->FindClass("dalvik/system/InMemoryDexClassLoader");
        auto dexClInit = env->GetMethodID(dexClClass, "<init>",
                                          "(Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V");
        auto dexCl = env->NewObject(dexClClass, dexClInit, buf, systemClassLoader);

        // Load the class
        LOGD("load class");
        auto loadClass = env->GetMethodID(clClass, "loadClass",
                                          "(Ljava/lang/String;)Ljava/lang/Class;");
        auto entryClassName = env->NewStringUTF("dev.kdrag0n.safetynetfix.EntryPoint");
        auto entryClassObj = env->CallObjectMethod(dexCl, loadClass, entryClassName);

        // Call init. Static initializers don't run when merely calling loadClass from JNI.
        LOGD("call init");
        auto entryClass = (jclass)entryClassObj;
        auto entryInit = env->GetStaticMethodID(entryClass, "init", "()V");
        env->CallStaticVoidMethod(entryClass, entryInit);
    }
};

REGISTER_ZYGISK_MODULE(SafetyNetFixModule)