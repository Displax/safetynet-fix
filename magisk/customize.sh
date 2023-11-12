if $BOOTMODE; then
    ui_print "- Installing from Magisk / KernelSU app"
else
    ui_print "*********************************************************"
    ui_print "! Install from recovery is NOT supported"
    ui_print "! Recovery sucks"
    ui_print "! Please install from Magisk / KernelSU app"
    abort    "*********************************************************"
fi

# Android < 8.0 | No Zygisk
if [ "$API" -lt 26 ]; then
    ui_print "*********************************************************"
    ui_print "! Functionality is limited on Android 7 and older"
    ui_print "! Hardware-backed attestation will not be disabled"
    ui_print "! MODEL & FINGERPRINT props will not be modified"
    ui_print "*********************************************************"

    # Remove Zygisk part but keep props and scripts
    rm -rf "$MODPATH/zygisk"
fi
