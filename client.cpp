/*
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#include "client.h"

#include <stdio.h>

#include <QTimerEvent>
#include <QDebug>

Client::Client(QObject *parent) :
    QObject(parent),
    mServer(0),
    mServiceTimer(0)
{
    mClient = enet_host_create(NULL,  // create a client host
                               1,     // only allow 1 outgoing connection
                               0, 0); // no bandwidth limits
}

Client::~Client()
{
    if (mClient)
        enet_host_destroy(mClient);
}

Client::State Client::state() const
{
    if (!mServer)
        return Unconnected;

    switch (mServer->state) {
    case ENET_PEER_STATE_CONNECTED:
    case ENET_PEER_STATE_CONNECTION_SUCCEEDED:     // ?
    case ENET_PEER_STATE_DISCONNECT_LATER:
    case ENET_PEER_STATE_DISCONNECTING:
    case ENET_PEER_STATE_ACKNOWLEDGING_CONNECT:    // ?
    case ENET_PEER_STATE_ACKNOWLEDGING_DISCONNECT: // ?
        return Connected;
    case ENET_PEER_STATE_CONNECTING:
    case ENET_PEER_STATE_CONNECTION_PENDING:       // ?
        return Connecting;
    case ENET_PEER_STATE_DISCONNECTED:
    case ENET_PEER_STATE_ZOMBIE:
    default:
        return Unconnected;
    }
}

void Client::connectToServer(const QString &host, quint16 port)
{
    if (mServer)
        enet_peer_disconnect_now(mServer, 0);

    qDebug() << Q_FUNC_INFO << host << port;
    ENetAddress address;
    // TODO: Make the host resolve step asynchroneous
    enet_address_set_host(&address, host.toLatin1());
    address.port = port;

    mServer = enet_host_connect(mClient, &address, 1);
    if (!mServer)
        qWarning() << "No available peers for initiating an ENet connection.";

    if (!mServiceTimer)
        mServiceTimer = startTimer(100);
}

void Client::disconnectFromServer()
{
    qDebug() << Q_FUNC_INFO;
    enet_peer_disconnect(mServer, 0);

    // TODO: enet_peer_reset if no ENET_EVENT_TYPE_DISCONNECT within 3 seconds
}

void Client::send(const QByteArray &data)
{
    if (!mServer)
        return;

    ENetPacket *packet = enet_packet_create(data.constData(),
                                            data.length(),
                                            ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(mServer, 0, packet);
}

void Client::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mServiceTimer)
        service();
}

void Client::service()
{
    if (!mClient)
        return;

    ENetEvent event;

    // Don't wait for events. This method is called often enough every second.
    while (enet_host_service(mClient, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            emit connected();
            break;

        case ENET_EVENT_TYPE_RECEIVE: {
            printf("A packet of length %u containing %s was received on channel %u.\n",
                   event.packet->dataLength,
                   event.packet->data,
                   event.channelID);
            fflush(stdout);

            const QByteArray message(QByteArray::fromRawData(
                    reinterpret_cast<char *>(event.packet->data),
                    event.packet->dataLength));

            emit messageReceived(message);

            enet_packet_destroy(event.packet);
        }
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            emit disconnected();
            break;

        case ENET_EVENT_TYPE_NONE:
            break; // Can never happen, but avoids compiler warning
        }
    }
}
