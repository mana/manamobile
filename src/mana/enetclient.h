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

#include <QObject>

#include <enet/enet.h>

class QHostInfo;

namespace Mana {

class ENetClient;
class MessageIn;
class MessageOut;

/**
 * A simple abstraction of an ENet based client.
 *
 * To create a functioning ENet based client, subclass this class and implement
 * the virtual messageReceived() method.
 */
class ENetClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY stateChanged)

    Q_ENUMS(State)

public:
    enum State {
        Disconnected,
        HostLookup,
        Connecting,
        Connected,
        Disconnecting
    };

    ENetClient(QObject *parent = 0);
    ~ENetClient();

    /**
     * A client is null when ENet failed to create a host for it. A null
     * client can't be used.
     */
    bool isNull() const { return mHost == 0; }

    State state() const { return mState; }

    bool isConnected() const { return mState == Connected; }

    /**
     * Connect to the server at the given \a hostName and \a port.
     *
     * The connect is asynchroneous. When the connection is established,
     * <code>connected()</code> will be called, and when there was a connection
     * error, <codE>disconnected()</code> will be called.
     */
    Q_INVOKABLE void connect(const QString &hostName, quint16 port);

    /**
     * Disconnect from the currently connected server.
     *
     * The disconnect is asynchroneous.
     */
    Q_INVOKABLE void disconnect();

    /**
     * Sends the given \a message to the server over the given \a channel
     * (0 by default).
     */
    void send(const MessageOut &message, unsigned char channel = 0);

    /**
     * Send and receive network packets.
     */
    Q_INVOKABLE void service();

signals:
    void connected();
    void disconnected();

    void stateChanged(ENetClient::State state);

protected:
    virtual void messageReceived(MessageIn &message) = 0;

private slots:
    void startConnecting(const QHostInfo &hostInfo);

private:
    void setState(State state);

    ENetHost *mHost;
    ENetPeer *mPeer;
    State mState;
    quint16 mPort;
};

} // namespace Mana

#endif // ENETCLIENT_H
