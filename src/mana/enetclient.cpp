/*
 *  manalib
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *
 *  This file is part of manalib.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "enetclient.h"

#include "messagein.h"
#include "messageout.h"

#include <QHostAddress>
#include <QHostInfo>
#include <QtEndian>

enum {
    debug_enetclient = 0
};

namespace Mana {

ENetClient::ENetClient(QObject *parent)
    : QObject(parent)
    , mPeer(0)
    , mState(Disconnected)
    , mPort(0)
{
    mHost = enet_host_create(NULL,  // create a client host
                             1,     // only allow 1 outgoing connection
                             0,     // no channel limit
                             0, 0); // no bandwidth limits
}

ENetClient::~ENetClient()
{
    if (mHost)
        enet_host_destroy(mHost);
}

void ENetClient::connect(const QString &hostName, quint16 port)
{
    // Force a quick disconnect if a server is already connected
    if (mPeer) {
        enet_peer_disconnect_now(mPeer, 0);
        mPeer = 0;
    }

    mPort = port;
    qDebug() << Q_FUNC_INFO << hostName << mPort;

    QHostAddress address;
    if (address.setAddress(hostName)) {
        // No need to lookup host
        QHostInfo hostInfo;
        hostInfo.setAddresses(QList<QHostAddress>() << address);
        startConnecting(hostInfo);
    }

    setState(HostLookup);
    QHostInfo::lookupHost(hostName, this, SLOT(startConnecting(QHostInfo)));
}

void ENetClient::disconnect()
{
    if (!mPeer)
        return;

    enet_peer_disconnect(mPeer, 0);
    setState(Disconnecting);
    // TODO: enet_peer_reset if no ENET_EVENT_TYPE_DISCONNECT within 3 seconds
}

void ENetClient::send(const MessageOut &message, unsigned char channel)
{
    if (mState != Connected) {
        qWarning() << "(ENetClient::send) Not connected!";
        return;
    }

    if (debug_enetclient)
        qDebug() << "(ENetClient::send) Sending " << message;

    ENetPacket *packet;
    packet = enet_packet_create(message.data(),
                                message.length(),
                                ENET_PACKET_FLAG_RELIABLE);
    if (packet)
        enet_peer_send(mPeer, channel, packet);
}

void ENetClient::service()
{
    if (isNull())
        return;

    ENetEvent event;

    while (enet_host_service(mHost, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            setState(Connected);
            emit connected();
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            mPeer = 0;
            setState(Disconnected);
            emit disconnected();
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            if (event.packet->dataLength < 2)
            {
                qWarning() << "(ENetClient::service) Warning: received a"
                        "packet that was too short!";
            }
            else
            {
                MessageIn message(reinterpret_cast<char*>(event.packet->data),
                                  event.packet->dataLength);

                if (debug_enetclient)
                    qDebug() << "(ENetClient::service) Received " << message;

                messageReceived(message);
            }

            enet_packet_destroy(event.packet);
            break;

        case ENET_EVENT_TYPE_NONE:
            break; // Can never happen, but avoids compiler warning
        }
    }
}

void ENetClient::setState(State state)
{
    if (mState == state)
        return;

    mState = state;
    emit stateChanged(mState);
}

void ENetClient::startConnecting(const QHostInfo &hostInfo)
{
    if (mState != HostLookup)
        return;

    const QList<QHostAddress> addresses = hostInfo.addresses();
    if (addresses.isEmpty()) {
        // TODO: Have a host lookup error for this case
        setState(Disconnected);
        return;
    }

    // TODO: Support hosts with multiple IP addresses?
    const QHostAddress &address = addresses.first();

    ENetAddress enetAddress;
    enetAddress.host = qToBigEndian(address.toIPv4Address());
    enetAddress.port = mPort;

    mPeer = enet_host_connect(mHost, &enetAddress, 1, 0);
    if (!mPeer) {
        qWarning() << "(ENetClient::connect) Warning: No available peers for "
                    "initiating an ENet connection.";
        setState(Disconnected);
        emit disconnected();
    } else {
        setState(Connecting);
    }
}

} // namespace Mana
