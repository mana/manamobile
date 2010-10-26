# Simple .pro file for including ENet directly in the project

DEPENDPATH += $$PWD/include/enet
INCLUDEPATH += $$PWD/include

DEFINES += HAS_GETHOSTBYADDR_R=1 \
           HAS_GETHOSTBYNAME_R=1 \
           HAS_POLL=1 \
           HAS_FCNTL=1 \
           HAS_INET_PTON=1 \
           HAS_INET_NTOP=1 \
           HAS_MSGHDR_FLAGS=1 \
           HAS_SOCKLEN_T=1

win32:LIBS += -lws2_32 -lwinmm

HEADERS += $$PWD/include/enet/callbacks.h \
           $$PWD/include/enet/enet.h \
           $$PWD/include/enet/list.h \
           $$PWD/include/enet/protocol.h \
           $$PWD/include/enet/time.h \
           $$PWD/include/enet/types.h \
           $$PWD/include/enet/unix.h \
           $$PWD/include/enet/utility.h \
           $$PWD/include/enet/win32.h
SOURCES += $$PWD/callbacks.c \
           $$PWD/compress.c \
           $$PWD/host.c \
           $$PWD/list.c \
           $$PWD/packet.c \
           $$PWD/peer.c \
           $$PWD/protocol.c \
           $$PWD/unix.c \
           $$PWD/win32.c
