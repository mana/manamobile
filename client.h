/**
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
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
