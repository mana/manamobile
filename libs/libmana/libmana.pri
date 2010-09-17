INCLUDEPATH += $$PWD/include

HEADERS += $$PWD/include/mana/protocol.h \
    $$PWD/include/mana/manaclient.h \
    $$PWD/include/mana/accounthandlerinterface.h \
    $$PWD/include/mana/gamehandlerinterface.h \
    $$PWD/include/mana/chathandlerinterface.h \
    $$PWD/messagein.h \
    $$PWD/enetclient.h \
    $$PWD/messageout.h \
    $$PWD/sha256.h \
    $$PWD/accountclient.h \
    $$PWD/gameclient.h \
    $$PWD/chatclient.h

SOURCES += $$PWD/manaclient.cpp \
    $$PWD/messagein.cpp \
    $$PWD/enetclient.cpp \
    $$PWD/messageout.cpp \
    $$PWD/sha256.cpp \
    $$PWD/accountclient.cpp \
    $$PWD/gameclient.cpp \
    $$PWD/chatclient.cpp
