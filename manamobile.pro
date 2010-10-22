# Check the Qt version. If QT_VERSION is not set, it is probably Qt 3.
isEmpty(QT_VERSION) {
    error("QT_VERSION not defined. Mana Mobile does not work with Qt 3.")
}
contains(QT_VERSION, ^4\\.[0-6]\\..*) {
    message("Cannot build Mana Mobile with Qt version $$QT_VERSION")
    error("Use at least Qt 4.7")
}

# Add more folders to ship with the application, here
folder_main.source = qml/main
folder_main.target = qml
DEPLOYMENTFOLDERS = folder_main

# Avoid auto screen rotation
#DEFINES += ORIENTATIONLOCK

# Needs to be defined for Symbian
DEFINES += NETWORKACCESS

symbian:TARGET.UID3 = 0xE5aedbec

symbian:ICON = symbianicon.svg

# Define to enable the Qml Observer in debug mode
#DEFINES += QMLOBSERVER

QT += network
contains(QT_CONFIG, opengl): QT += opengl

include(libs/enet/enet.pri)
include(libs/libmana/libmana.pri)
include(libs/libtiled/libtiled.pri)
include(src/src.pri)

include(qmlapplicationviewer/qmlapplicationviewer.pri)

qtcAddDeployment()
