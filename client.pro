QT       += core gui network

TARGET = manamobile
TEMPLATE = app

include(enet/enet.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    serversettingsdialog.cpp \
    loginmanager.cpp \
    client.cpp \
    sha256.cpp

HEADERS  += mainwindow.h \
    serversettingsdialog.h \
    loginmanager.h \
    protocol.h \
    client.h \
    sha256.h

FORMS    += mainwindow.ui \
    serversettingsdialog.ui

CONFIG += mobility
MOBILITY =

symbian {
    TARGET.UID3 = 0xe3c00cfe
    TARGET.CAPABILITY += NetworkServices
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
