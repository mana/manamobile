#-------------------------------------------------
#
# Project created by QtCreator 2010-05-01T11:22:18
#
#-------------------------------------------------

QT       += core gui network

TARGET = manamobile
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversettingsdialog.cpp \
    loginmanager.cpp

HEADERS  += mainwindow.h \
    serversettingsdialog.h \
    loginmanager.h \
    protocol.h

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
