/**
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QObject>
#include <QTcpSocket>

class LoginManager : public QObject
{
    Q_OBJECT
public:
    explicit LoginManager(QObject *parent = 0);

    void connectToLoginServer(const QString &host, quint16 port);

    void login(const QString &username, const QString &password);


signals:
    void connected();
    void disconnected();

private slots:
    void onConnected();
    void onStateChanged(QAbstractSocket::SocketState state);
    void onReadyRead();

private:
    QTcpSocket *mSocket;
    QByteArray mInputBuffer;
};

#endif // LOGINMANAGER_H
