TEMPLATE = lib
CONFIG -= qt
CONFIG += static
TARGET = mana
INCLUDEPATH += include
DESTDIR=.lib

# Hide build artifacts
MOC_DIR = .moc
UI_DIR = .uic
RCC_DIR = .rcc
OBJECTS_DIR = .obj

include(../enet/enet.pri)

HEADERS += include/mana/protocol.h \
    include/mana/manaclient.h \
    include/mana/accounthandlerinterface.h \
    include/mana/gamehandlerinterface.h \
    include/mana/chathandlerinterface.h \
    messagein.h \
    enetclient.h \
    messageout.h \
    sha256.h \
    accountclient.h \
    gameclient.h \
    chatclient.h
SOURCES += manaclient.cpp \
    messagein.cpp \
    enetclient.cpp \
    messageout.cpp \
    sha256.cpp \
    accountclient.cpp \
    gameclient.cpp \
    chatclient.cpp
