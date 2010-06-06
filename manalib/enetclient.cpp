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

#include <iostream>

enum {
    debug_enetclient = 0
};

namespace Mana {

ENetClient::ENetClient()
    : mPeer(0)
{
    mHost = enet_host_create(NULL,  // create a client host
                             1,     // only allow 1 outgoing connection
                             0, 0); // no bandwidth limits
}

ENetClient::~ENetClient()
{
    if (mHost)
        enet_host_destroy(mHost);
}

bool ENetClient::isConnected() const
{
    return mPeer && mPeer->state == ENET_PEER_STATE_CONNECTED;
}

void ENetClient::connect(const char *hostName, unsigned short port)
{
    // Force a quick disconnect if a server is already connected
    if (mPeer)
        enet_peer_disconnect_now(mPeer, 0);

    ENetAddress address;
    // TODO: Make the host resolve step asynchroneous
    enet_address_set_host(&address, hostName);
    address.port = port;

    mPeer = enet_host_connect(mHost, &address, 1);
    if (!mPeer)
    {
        std::cerr << "(ENetClient::connect) Warning: No available peers for "
                "initiating an ENet connection." << std::endl;
        disconnected();
    }
}

void ENetClient::disconnect()
{
    if (mPeer)
        enet_peer_disconnect(mPeer, 0);
    // TODO: enet_peer_reset if no ENET_EVENT_TYPE_DISCONNECT within 3 seconds
}

void ENetClient::send(const MessageOut &message, unsigned char channel)
{
    if (!isConnected()) {
        std::cerr << "(ENetClient::send) Not connected!" << std::endl;
        return;
    }

    if (debug_enetclient)
        std::cout << "(ENetClient::send) Sending " << message << std::endl;

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
            connected();
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            disconnected();
            mPeer = 0;
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            if (event.packet->dataLength < 2)
            {
                std::cerr << "(ENetClient::service) Warning: received a"
                        "packet that was too short!" << std::endl;
            }
            else
            {
                MessageIn message(reinterpret_cast<char*>(event.packet->data),
                                  event.packet->dataLength);

                if (debug_enetclient)
                    std::cout << "(ENetClient::service) Received " << message
                            << std::endl;

                messageReceived(message);
            }

            enet_packet_destroy(event.packet);
            break;

        case ENET_EVENT_TYPE_NONE:
            break; // Can never happen, but avoids compiler warning
        }
    }
}

} // namespace Mana
