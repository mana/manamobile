/*
 *  Mana Mobile
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *
 *  This file is part of Mana Mobile.
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

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include <enet/enet.h>

class Client : public QObject
{
    Q_OBJECT

public:
    enum State {
        Unconnected,
        Connecting,
        Connected
    };

    explicit Client(QObject *parent = 0);
    ~Client();

    /**
     * A client is null when ENet failed to create a host for it. A null client
     * can't be used.
     */
    bool isNull() const { return mClient == 0; }

    /**
     * Returns the client state.
     */
    State state() const;

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    void send(const QByteArray &data);

signals:
    void connected();
    void disconnected();

    /**
     * Emitted when a message is received. The byte array is only valid while
     * this signal is being emitted. If you need the data around longer, make
     * an explicitly detached copy.
     */
    void messageReceived(const QByteArray &message);

protected:
    void timerEvent(QTimerEvent *);

private slots:
    void service();

private:
    ENetHost *mClient;
    ENetPeer *mServer;
    int mServiceTimer;
};

#endif // CLIENT_H
