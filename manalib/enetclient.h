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

#ifndef ENETCLIENT_H
#define ENETCLIENT_H

#include <string>

#include <enet/enet.h>

namespace Mana {

class ENetClient;
class MessageIn;
class MessageOut;

/**
 * An interface that allows responding to client events.
 */
class ENetClientEventHandler
{
public:
    virtual void connected() = 0;
    virtual void disconnected() = 0;
    virtual void messageReceived(MessageIn &message) = 0;
};

/**
 * A simple abstraction of an ENet based client.
 */
class ENetClient
{
public:
    /**
     * Constructor, taking the handler that will handle client events.
     */
    ENetClient(ENetClientEventHandler *handler);
    ~ENetClient();

    /**
     * A client is null when ENet failed to create a host for it. A null
     * client can't be used.
     */
    bool isNull() const { return mHost == 0; }

    bool isConnected() const;

    /**
     * Connect to the server at the given \a hostName and \a port.
     *
     * The connect is asynchroneous. The ENetClientEventHandler will receive a
     * call to <code>connected()</code> when a connection is established, or a
     * call to <codE>disconnected()</code> when there was a connection error.
     */
    void connect(const char *hostName, unsigned short port);

    /**
     * Disconnect from the currently connected server.
     *
     * The disconnect is asynchroneous.
     */
    void disconnect();

    /**
     * Sends the given \a message to the server over the given \a channel
     * (0 by default).
     */
    void send(const MessageOut &message, unsigned char channel = 0);

    /**
     * Send and receive network packets.
     */
    void service();

private:
    ENetClientEventHandler *mHandler;
    ENetHost *mHost;
    ENetPeer *mPeer;
};

} // namespace Mana

#endif // ENETCLIENT_H
