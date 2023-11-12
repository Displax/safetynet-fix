## v3.0.0-MOD

* Fix new Play Integrity update, DEVICE verdict is green again.
* Strip native libs size.
* Unmap module from unstable GMS process.
* Spoof first_api_level to 32 if prop value is greater than 32 or prop doesn't exist.

## v2.4.0-MOD_2.0
* Fix KernelSU support. You still need "ZygiskOnKernelSU".
* Fix WiFi calling on some devices (and maybe other related issues) by moving changing *ro.product.first_api_level* from global namespace to GMS only.
* Pass *MEETS_STRONG_INTEGRITY*. By default you need device that launched with Android 13+ or custom ROM that manipulate with *ro.product.first_api_level* (>= 33 / or `null` / or not present).<br/>If you want to play with it more complex - use separate "-STRONG" build. NOTE: Various system issues are possible on "-STRONG" build!

## v2.4.0-MOD_1.3

* Fix "stat /sys/fs/selinux" access time reading. Also removed archaic MIUI cross-region shenanigans. Thanks to PR by aviraxp!
* Ignore props changing on Xiaomi.eu. This fixes randomly attestation failings. So strange ROM...
* Added Riru version back. Remember that is need old MagiskHide support (and adding com.google.android.gms/com.google.android.gms.unstable to HideList) !
* Added microG version. Note there is some strange behavior that GMS can crash while run SN attest more than 2 times in one session. Just do not do this. This behavior should not affect applications in real life.

## v2.4.0-MOD_1.2

* Fix crash and endless tests loop/failing on Android < 9.0 (bug from original version 2.4.0).
* Do not unpatch (revert) changes. To prevent possible tests failing after a while on some ROMs (cross conflicts).

## v2.4.0-MOD_1.1

* Fix KeyStore hook desynchronization (tests randomly failing problem).

## v2.4.0-MOD_1.0

* It is now based on top of original v2.4.0 codebase instead of v2.3.1, with adding new hiding algorithm for current realities and some code refreshing.
