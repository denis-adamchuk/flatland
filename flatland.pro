TEMPLATE = subdirs

SUBDIRS += lib.pro

contains(DEFINES, DESKTOP) {
SUBDIRS += \
    ui.pro \
    console2.pro \
    ut.pro
}
else {
SUBDIRS += \
    ui-android.pro
}
