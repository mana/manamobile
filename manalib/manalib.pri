LIB = libmana.a

win32-msvc* {
    LIB = mana.lib
}

INCLUDEPATH += $$PWD/include
LIBS += $$OUT_PWD/manalib/.lib/$$LIB

include(../enet/enet.pri)
