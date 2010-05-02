/**
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#include "loginmanager.h"

#include "protocol.h"

#include <QDebug>

LoginManager::LoginManager(QObject *parent) :
    QObject(parent),
    mSocket(new QTcpSocket(this))
{
    connect(mSocket, SIGNAL(connected()), SLOT(onConnected()));
    connect(mSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(mSocket, SIGNAL(readyRead()), SLOT(onReadyRead()));
    connect(mSocket, SIGNAL(disconnected()), SIGNAL(disconnected()));
}

void LoginManager::connectToLoginServer(const QString &host, quint16 port)
{
    if (mSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Already connected!";
        return;
    }

    mSocket->connectToHost(host, port);
}

void LoginManager::login(const QString &username, const QString &password)
{
    if (mSocket->state() != QTcpSocket::ConnectedState)
        return;

    // TODO: Write a small helper class for this stuff
    const QByteArray convertedUsername =
            username.toLatin1().leftJustified(24, '\0', true);
    const QByteArray convertedPassword =
            password.toLatin1().leftJustified(24, '\0', true);

    QByteArray loginMessage;
    QDataStream stream(&loginMessage, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << (quint16) 0x0064;
    stream << (quint32) 0;
    stream.writeRawData(convertedUsername.constData(),
                        convertedUsername.length());
    stream.writeRawData(convertedPassword.constData(),
                        convertedPassword.length());
    stream << (quint8) 0x02;
    mSocket->write(loginMessage);
}

void LoginManager::onConnected()
{
    qDebug() << Q_FUNC_INFO;

    QByteArray versionRequest;
    QDataStream stream(&versionRequest, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << (quint16) CMSG_SERVER_VERSION_REQUEST;
    mSocket->write(versionRequest);

    emit connected();
}

void LoginManager::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << Q_FUNC_INFO << state;
}

void LoginManager::onReadyRead()
{
    qDebug() << Q_FUNC_INFO;
    mInputBuffer += mSocket->readAll();
    // TODO: Deal with incoming bytes
}
