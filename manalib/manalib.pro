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

HEADERS += include/mana/protocol.h \
    include/mana/manaclient.h \
    include/mana/accounthandlerinterface.h \
    messagein.h \
    enetclient.h \
    messageout.h \
    sha256.h
SOURCES += manaclient.cpp \
    messagein.cpp \
    enetclient.cpp \
    messageout.cpp \
    sha256.cpp
