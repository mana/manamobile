TEMPLATE = lib
CONFIG += qt plugin

DESTDIR = ../qml/Mana/
TARGET = mana

QT += network qml quick

DEFINES += QT_NO_URL_CAST_FROM_STRING

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

include(enet/enet.pri)

INCLUDEPATH += .

SOURCES += \
    mana/accountclient.cpp \
    mana/attributelistmodel.cpp \
    mana/being.cpp \
    mana/beinglistmodel.cpp \
    mana/character.cpp \
    mana/characterlistmodel.cpp \
    mana/chatclient.cpp \
    mana/enetclient.cpp \
    mana/gameclient.cpp \
    mana/manaplugin.cpp \
    mana/mapitem.cpp \
    mana/messagein.cpp \
    mana/messageout.cpp \
    mana/monster.cpp \
    mana/npc.cpp \
    mana/npcdialogmanager.cpp \
    mana/resource/action.cpp \
    mana/resource/animation.cpp \
    mana/resource/attributedb.cpp \
    mana/resource/hairdb.cpp \
    mana/resource/imageresource.cpp \
    mana/resource/imageset.cpp \
    mana/resource/itemdb.cpp \
    mana/resource/monsterdb.cpp \
    mana/resource/npcdb.cpp \
    mana/resource/racedb.cpp \
    mana/resource/resource.cpp \
    mana/resource/spritedef.cpp \
    mana/resourcelistmodel.cpp \
    mana/resourcemanager.cpp \
    mana/settings.cpp \
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
    mana/accountclient.h \
    mana/attributelistmodel.h \
    mana/being.h \
    mana/beinglistmodel.h \
    mana/character.h \
    mana/characterlistmodel.h \
    mana/chatclient.h \
    mana/enetclient.h \
    mana/gameclient.h \
    mana/manaplugin.h \
    mana/mapitem.h \
    mana/messagein.h \
    mana/messageout.h \
    mana/monster.h \
    mana/npc.h \
    mana/npcdialogmanager.h \
    mana/protocol.h \
    mana/resource/action.h \
    mana/resource/animation.h \
    mana/resource/attributedb.h \
    mana/resource/hairdb.h \
    mana/resource/imageresource.h \
    mana/resource/imageset.h \
    mana/resource/itemdb.h \
    mana/resource/monsterdb.h \
    mana/resource/npcdb.h \
    mana/resource/racedb.h \
    mana/resource/resource.h \
    mana/resource/spritedef.h \
    mana/resourcelistmodel.h \
    mana/resourcemanager.h \
    mana/settings.h \
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


copy_qmldir.target = ../qml/Mana/qmldir
copy_qmldir.destination = ../qml/
copy_qmldir.path = $$_PRO_FILE_PWD_/mana/qml/Mana

copy_qmldir.commands = $(COPY_DIR) $$replace(copy_qmldir.path, /, $$QMAKE_DIR_SEP) $$replace(copy_qmldir.destination, /, $$QMAKE_DIR_SEP)
QMAKE_EXTRA_TARGETS += copy_qmldir
PRE_TARGETDEPS += $$copy_qmldir.target

OTHER_FILES += \
    mana/qml/Mana/ServerListModel.qml \
    mana/qml/Mana/qmldir
