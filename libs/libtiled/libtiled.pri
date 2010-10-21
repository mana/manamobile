INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!win32:LIBS += -lz
win32:INCLUDEPATH += $$(QTDIR)/src/3rdparty/zlib

SOURCES += $$PWD/isometricrenderer.cpp \
    $$PWD/layer.cpp \
    $$PWD/map.cpp \
    $$PWD/mapobject.cpp \
    $$PWD/objectgroup.cpp \
    $$PWD/orthogonalrenderer.cpp \
    $$PWD/tilelayer.cpp \
    $$PWD/tileset.cpp \
    $$PWD/mapreader.cpp \
    $$PWD/mapwriter.cpp \
    $$PWD/properties.cpp \
    $$PWD/compression.cpp

HEADERS += $$PWD/isometricrenderer.h \
    $$PWD/layer.h \
    $$PWD/map.h \
    $$PWD/mapobject.h \
    $$PWD/maprenderer.h \
    $$PWD/objectgroup.h \
    $$PWD/orthogonalrenderer.h \
    $$PWD/tile.h \
    $$PWD/tiled_global.h \
    $$PWD/tilelayer.h \
    $$PWD/tileset.h \
    $$PWD/mapreader.h \
    $$PWD/mapwriter.h \
    $$PWD/properties.h \
    $$PWD/object.h \
    $$PWD/compression.h
