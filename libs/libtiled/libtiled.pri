INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

macx:LIBS += -lz
win32:INCLUDEPATH += $$(QTDIR)/src/3rdparty/zlib

SOURCES += $$PWD/layer.cpp \
    $$PWD/map.cpp \
    $$PWD/mapobject.cpp \
    $$PWD/objectgroup.cpp \
    $$PWD/tilelayer.cpp \
    $$PWD/tileset.cpp \
    $$PWD/mapreader.cpp \
    $$PWD/properties.cpp \
    $$PWD/compression.cpp

HEADERS += $$PWD/layer.h \
    $$PWD/map.h \
    $$PWD/mapobject.h \
    $$PWD/objectgroup.h \
    $$PWD/tile.h \
    $$PWD/tiled_global.h \
    $$PWD/tilelayer.h \
    $$PWD/tileset.h \
    $$PWD/mapreader.h \
    $$PWD/properties.h \
    $$PWD/object.h \
    $$PWD/compression.h
