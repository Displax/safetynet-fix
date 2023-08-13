package dev.kdrag0n.safetynetfix.proxy

import android.os.Build
import dev.kdrag0n.safetynetfix.SecurityHooks
import dev.kdrag0n.safetynetfix.logDebug
import java.security.Provider
import kotlin.concurrent.thread

// This is mostly just a pass-through provider that exists to change the provider's ClassLoader.
// This works because Service looks up the class by name from the *provider* ClassLoader, not
// necessarily the bootstrap one.
class ProxyProvider(
    orig: Provider,
) : Provider(orig.name, orig.version, orig.info) {
    init {
        logDebug("Init proxy provider - wrapping $orig")

        putAll(orig)
        this["KeyStore.${SecurityHooks.PROVIDER_NAME}"] = ProxyKeyStoreSpi::class.java.name
    }

    override fun getService(type: String?, algorithm: String?): Service? {
        logDebug("Provider: get service - type=$type algorithm=$algorithm")
        if (type == "KeyStore" && Build.HOST != "xiaomi.eu") {

            val patchedProduct     = /* ro.product.name      */ "marlin"
            val patchedDevice      = /* ro.product.device    */ "marlin"
            val patchedModel       = /* ro.product.model     */ "Pixel XL"
            val patchedFingerprint = /* ro.build.fingerprint */ "google/marlin/marlin:7.1.2/NJH47F/4146041:user/release-keys"

            logDebug("Patch PRODUCT prop. Set it to: $patchedProduct")
            Build::class.java.getDeclaredField("PRODUCT").let { field ->
                field.isAccessible = true
                field.set(null, patchedProduct)
            }
            logDebug("Patch DEVICE prop. Set it to: $patchedDevice")
            Build::class.java.getDeclaredField("DEVICE").let { field ->
                field.isAccessible = true
                field.set(null, patchedDevice)
            }
            logDebug("Patch MODEL prop. Set it to: $patchedModel")
            Build::class.java.getDeclaredField("MODEL").let { field ->
                field.isAccessible = true
                field.set(null, patchedModel)
            }
            logDebug("Patch FINGERPRINT prop. Set it to: $patchedFingerprint")
            Build::class.java.getDeclaredField("FINGERPRINT").let { field ->
                field.isAccessible = true
                field.set(null, patchedFingerprint)
            }
        }
        return super.getService(type, algorithm)
    }

    override fun getServices(): MutableSet<Service>? {
        logDebug("Get services")
        return super.getServices()
    }
}
