TEMPLATE = lib
CONFIG += qt plugin
linux*:QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF

DESTDIR = qml/Mana/
TARGET = mana

QT += network qml quick

DEFINES += QT_NO_URL_CAST_FROM_STRING

INCLUDEPATH += src

# Pretend we're building the Tiled library to make sure it doesn't try to
# mark the Tiled classes as being imported.
DEFINES += TILED_LIBRARY

win32:INCLUDEPATH += $$(QTDIR)/src/3rdparty/zlib
LIBS += -lz

!win32-msvc2010 {
    # Silence compile warnings in ENet code
    # (this effectively excludes those types of warnings for C code)
    CONFIG += warn_off
    QMAKE_CFLAGS += -Wall -W -Wno-switch -Wno-unknown-pragmas -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wall -W
}

include(enet/enet.pri)

INCLUDEPATH += .

SOURCES += \
    mana/abilitylistmodel.cpp \
    mana/accountclient.cpp \
    mana/attributelistmodel.cpp \
    mana/being.cpp \
    mana/beinglistmodel.cpp \
    mana/character.cpp \
    mana/characterlistmodel.cpp \
    mana/chatclient.cpp \
    mana/collisionhelper.cpp \
    mana/droplistmodel.cpp \
    mana/enetclient.cpp \
    mana/gameclient.cpp \
    mana/inventorylistmodel.cpp \
    mana/logicdriver.cpp \
    mana/manaplugin.cpp \
    mana/mapitem.cpp \
    mana/messagein.cpp \
    mana/messageout.cpp \
    mana/monster.cpp \
    mana/npc.cpp \
    mana/questloglistmodel.cpp \
    mana/resource/abilitydb.cpp \
    mana/resource/action.cpp \
    mana/resource/animation.cpp \
    mana/resource/attributedb.cpp \
    mana/resource/hairdb.cpp \
    mana/resource/imageresource.cpp \
    mana/resource/imageset.cpp \
    mana/resource/itemdb.cpp \
    mana/resource/mapresource.cpp \
    mana/resource/monsterdb.cpp \
    mana/resource/npcdb.cpp \
    mana/resource/racedb.cpp \
    mana/resource/resource.cpp \
    mana/resource/spritedef.cpp \
    mana/resourcelistmodel.cpp \
    mana/resourcemanager.cpp \
    mana/settings.cpp \
    mana/shoplistmodel.cpp \
    mana/spriteitem.cpp \
    mana/spritelistmodel.cpp \
    mana/tilelayeritem.cpp \
    tiled/compression.cpp \
    tiled/gidmapper.cpp \
    tiled/imagelayer.cpp \
    tiled/isometricrenderer.cpp \
    tiled/layer.cpp \
    tiled/map.cpp \
    tiled/mapobject.cpp \
    tiled/mapreader.cpp \
    tiled/maprenderer.cpp \
    tiled/mapwriter.cpp \
    tiled/objectgroup.cpp \
    tiled/orthogonalrenderer.cpp \
    tiled/properties.cpp \
    tiled/staggeredrenderer.cpp \
    tiled/tile.cpp \
    tiled/tilelayer.cpp \
    tiled/tileset.cpp

HEADERS += \
    durationlogger.h \
    mana/abilitylistmodel.h \
    mana/accountclient.h \
    mana/attributelistmodel.h \
    mana/being.h \
    mana/beinglistmodel.h \
    mana/character.h \
    mana/characterlistmodel.h \
    mana/chatclient.h \
    mana/collisionhelper.h \
    mana/droplistmodel.h \
    mana/enetclient.h \
    mana/gameclient.h \
    mana/inventorylistmodel.h \
    mana/logicdriver.h \
    mana/manaplugin.h \
    mana/mapitem.h \
    mana/messagein.h \
    mana/messageout.h \
    mana/monster.h \
    mana/npc.h \
    mana/protocol.h \
    mana/questloglistmodel.h \
    mana/resource/abilitydb.h \
    mana/resource/action.h \
    mana/resource/animation.h \
    mana/resource/attributedb.h \
    mana/resource/hairdb.h \
    mana/resource/imageresource.h \
    mana/resource/imageset.h \
    mana/resource/itemdb.h \
    mana/resource/mapresource.h \
    mana/resource/monsterdb.h \
    mana/resource/npcdb.h \
    mana/resource/racedb.h \
    mana/resource/resource.h \
    mana/resource/spritedef.h \
    mana/resourcelistmodel.h \
    mana/resourcemanager.h \
    mana/settings.h \
    mana/shoplistmodel.h \
    mana/spriteitem.h \
    mana/spritelistmodel.h \
    mana/tilelayeritem.h \
    mana/xmlreader.h \
    safeassert.h \
    tiled/compression.h \
    tiled/gidmapper.h \
    tiled/imagelayer.h \
    tiled/isometricrenderer.h \
    tiled/layer.h \
    tiled/logginginterface.h \
    tiled/map.h \
    tiled/mapobject.h \
    tiled/mapreader.h \
    tiled/mapreaderinterface.h \
    tiled/maprenderer.h \
    tiled/mapwriter.h \
    tiled/mapwriterinterface.h \
    tiled/objectgroup.h \
    tiled/object.h \
    tiled/orthogonalrenderer.h \
    tiled/properties.h \
    tiled/staggeredrenderer.h \
    tiled/terrain.h \
    tiled/tiled.h \
    tiled/tiled_global.h \
    tiled/tile.h \
    tiled/tilelayer.h \
    tiled/tileset.h

folder_Mana.source = mana/qml/Mana
folder_Mana.target = qml
DEPLOYMENTFOLDERS = folder_Mana

# Please do not modify the following two lines. Required for deployment.
include(../example/qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
