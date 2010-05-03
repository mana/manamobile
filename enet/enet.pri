LIB = libenet.a

win32-msvc* {
    LIB = enet.lib
    LIBS += ws2_32.lib winmm.lib
}

INCLUDEPATH += $$PWD/include
LIBS += $$OUT_PWD/enet/.lib/$$LIB
