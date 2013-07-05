# Check the Qt version. If QT_VERSION is not set, it is probably Qt 3.
isEmpty(QT_VERSION) {
    error("QT_VERSION not defined. Mana Mobile requires Qt 5.")
}
contains(QT_VERSION, ^4\\..*) {
    message("Cannot build Mana Mobile with Qt version $$QT_VERSION")
    error("Use at least Qt 5.0")
}

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += src example
