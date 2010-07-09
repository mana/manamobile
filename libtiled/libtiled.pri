LIB = libtiled.a

win32-msvc* {
    LIB = tiled.lib
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
LIBS += $$OUT_PWD/libtiled/.lib/$$LIB
