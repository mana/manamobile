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

# Define QMLJSDEBUGGER to allow debugging of QML
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

QT += network
contains(QT_CONFIG, opengl): QT += opengl

INCLUDEPATH += src

# Pretend we're building the Tiled library to make sure it doesn't try to
# mark the Tiled classes as being imported.
DEFINES += TILED_LIBRARY

win32:INCLUDEPATH += $$(QTDIR)/src/3rdparty/zlib
else:LIBS += -lz

!win32-msvc2010 {
    # Silence compile warnings in ENet code
    # (this effectively excludes those two types of warnings for C code)
    CONFIG += warn_off
    QMAKE_CFLAGS += -Wall -W -Wno-switch -Wno-unknown-pragmas
    QMAKE_CXXFLAGS += -Wall -W
}

# The .cpp file which was generated for your project. Feel free to hack it.
include(src/enet/enet.pri)

SOURCES += src/main.cpp \
    src/beingitem.cpp \
    src/mana/accountclient.cpp \
    src/mana/being.cpp \
    src/mana/characterlistmodel.cpp \
    src/mana/chatclient.cpp \
    src/mana/enetclient.cpp \
    src/mana/gameclient.cpp \
    src/mana/messagein.cpp \
    src/mana/messageout.cpp \
    src/mana/resource/action.cpp \
    src/mana/resource/animation.cpp \
    src/mana/resource/imageset.cpp \
    src/mana/resource/itemdb.cpp \
    src/mana/resource/pixmapresource.cpp \
    src/mana/resource/resource.cpp \
    src/mana/resource/spritedef.cpp \
    src/mana/settings.cpp \
    src/mana/sha256.cpp \
    src/mapitem.cpp \
    src/resourcemanager.cpp \
    src/spriteitem.cpp \
    src/tiled/compression.cpp \
    src/tiled/gidmapper.cpp \
    src/tiled/imagelayer.cpp \
    src/tiled/isometricrenderer.cpp \
    src/tiled/layer.cpp \
    src/tiled/map.cpp \
    src/tiled/mapobject.cpp \
    src/tiled/mapreader.cpp \
    src/tiled/maprenderer.cpp \
    src/tiled/mapwriter.cpp \
    src/tiled/objectgroup.cpp \
    src/tiled/orthogonalrenderer.cpp \
    src/tiled/properties.cpp \
    src/tiled/staggeredrenderer.cpp \
    src/tiled/tilelayer.cpp \
    src/tiled/tileset.cpp \
    src/tilelayeritem.cpp \
    src/mana/beinglistmodel.cpp

HEADERS += \
    src/beingitem.h \
    src/durationlogger.h \
    src/mana/accountclient.h \
    src/mana/being.h \
    src/mana/chatclient.h \
    src/mana/enetclient.h \
    src/mana/gameclient.h \
    src/mana/messagein.h \
    src/mana/messageout.h \
    src/mana/protocol.h \
    src/mana/resource/action.h \
    src/mana/resource/animation.h \
    src/mana/resource/imageset.h \
    src/mana/resource/itemdb.h \
    src/mana/resource/pixmapresource.h \
    src/mana/resource/resource.h \
    src/mana/resource/spritedef.h \
    src/mana/settings.h \
    src/mana/sha256.h \
    src/mana/xmlreader.h \
    src/mapitem.h \
    src/resourcemanager.h \
    src/safeassert.h \
    src/spriteitem.h \
    src/tiled/compression.h \
    src/tiled/gidmapper.h \
    src/tiled/imagelayer.h \
    src/tiled/isometricrenderer.h \
    src/tiled/layer.h \
    src/tiled/map.h \
    src/tiled/mapobject.h \
    src/tiled/mapreader.h \
    src/tiled/mapreaderinterface.h \
    src/tiled/maprenderer.h \
    src/tiled/mapwriter.h \
    src/tiled/mapwriterinterface.h \
    src/tiled/objectgroup.h \
    src/tiled/object.h \
    src/tiled/orthogonalrenderer.h \
    src/tiled/properties.h \
    src/tiled/staggeredrenderer.h \
    src/tiled/terrain.h \
    src/tiled/tiled_global.h \
    src/tiled/tile.h \
    src/tiled/tilelayer.h \
    src/tiled/tileset.h \
    src/tilelayeritem.h \
    src/mana/characterlistmodel.h \
    src/mana/beinglistmodel.h


# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
