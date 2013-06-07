# Check the Qt version. If QT_VERSION is not set, it is probably Qt 3.
isEmpty(QT_VERSION) {
    error("QT_VERSION not defined. Mana Mobile requires Qt 5.")
}
contains(QT_VERSION, ^4\\..*) {
    message("Cannot build Mana Mobile with Qt version $$QT_VERSION")
    error("Use at least Qt 5.0")
}

# Add more folders to ship with the application, here
folder_main.source = qml/main
folder_main.target = qml
DEPLOYMENTFOLDERS = folder_main

DEFINES += QT_NO_URL_CAST_FROM_STRING

QT += network

INCLUDEPATH += src

# Pretend we're building the Tiled library to make sure it doesn't try to
# mark the Tiled classes as being imported.
DEFINES += TILED_LIBRARY

win32:INCLUDEPATH += $$(QTDIR)/src/3rdparty/zlib
else:LIBS += -lz

!win32-msvc2010 {
    # Silence compile warnings in ENet code
    # (this effectively excludes those types of warnings for C code)
    CONFIG += warn_off
    QMAKE_CFLAGS += -Wall -W -Wno-switch -Wno-unknown-pragmas -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wall -W
}

# The .cpp file which was generated for your project. Feel free to hack it.
include(src/enet/enet.pri)

SOURCES += src/main.cpp \
    src/mana/accountclient.cpp \
    src/mana/attributelistmodel.cpp \
    src/mana/being.cpp \
    src/mana/beinglistmodel.cpp \
    src/mana/character.cpp \
    src/mana/characterlistmodel.cpp \
    src/mana/chatclient.cpp \
    src/mana/enetclient.cpp \
    src/mana/gameclient.cpp \
    src/mana/mapitem.cpp \
    src/mana/messagein.cpp \
    src/mana/messageout.cpp \
    src/mana/monster.cpp \
    src/mana/npc.cpp \
    src/mana/npcdialogmanager.cpp \
    src/mana/resource/action.cpp \
    src/mana/resource/animation.cpp \
    src/mana/resource/attributedb.cpp \
    src/mana/resource/hairdb.cpp \
    src/mana/resource/imageresource.cpp \
    src/mana/resource/imageset.cpp \
    src/mana/resource/itemdb.cpp \
    src/mana/resource/monsterdb.cpp \
    src/mana/resource/npcdb.cpp \
    src/mana/resource/racedb.cpp \
    src/mana/resource/resource.cpp \
    src/mana/resource/spritedef.cpp \
    src/mana/resourcelistmodel.cpp \
    src/mana/resourcemanager.cpp \
    src/mana/settings.cpp \
    src/mana/spriteitem.cpp \
    src/mana/spritelistmodel.cpp \
    src/mana/tilelayeritem.cpp \
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
    src/tiled/tile.cpp \
    src/tiled/tilelayer.cpp \
    src/tiled/tileset.cpp

HEADERS += \
    src/durationlogger.h \
    src/mana/accountclient.h \
    src/mana/attributelistmodel.h \
    src/mana/being.h \
    src/mana/beinglistmodel.h \
    src/mana/character.h \
    src/mana/characterlistmodel.h \
    src/mana/chatclient.h \
    src/mana/enetclient.h \
    src/mana/gameclient.h \
    src/mana/mapitem.h \
    src/mana/messagein.h \
    src/mana/messageout.h \
    src/mana/monster.h \
    src/mana/npc.h \
    src/mana/npcdialogmanager.h \
    src/mana/protocol.h \
    src/mana/resource/action.h \
    src/mana/resource/animation.h \
    src/mana/resource/attributedb.h \
    src/mana/resource/hairdb.h \
    src/mana/resource/imageresource.h \
    src/mana/resource/imageset.h \
    src/mana/resource/itemdb.h \
    src/mana/resource/monsterdb.h \
    src/mana/resource/npcdb.h \
    src/mana/resource/racedb.h \
    src/mana/resource/resource.h \
    src/mana/resource/spritedef.h \
    src/mana/resourcelistmodel.h \
    src/mana/resourcemanager.h \
    src/mana/settings.h \
    src/mana/spriteitem.h \
    src/mana/spritelistmodel.h \
    src/mana/tilelayeritem.h \
    src/mana/xmlreader.h \
    src/safeassert.h \
    src/tiled/compression.h \
    src/tiled/gidmapper.h \
    src/tiled/imagelayer.h \
    src/tiled/isometricrenderer.h \
    src/tiled/layer.h \
    src/tiled/logginginterface.h \
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
    src/tiled/tiled.h \
    src/tiled/tiled_global.h \
    src/tiled/tile.h \
    src/tiled/tilelayer.h \
    src/tiled/tileset.h


# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/values-pl/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-el/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-id/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fr/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java
