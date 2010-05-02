/**
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#include "loginmanager.h"

#include "client.h"
#include "protocol.h"

#include <QIODevice>
#include <QDebug>

LoginManager::LoginManager(QObject *parent) :
    QObject(parent),
    mClient(new Client(this))
{
    connect(mClient, SIGNAL(connected()), SIGNAL(connected()));
    connect(mClient, SIGNAL(disconnected()), SIGNAL(disconnected()));
//    connect(mClient, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void LoginManager::connectToLoginServer(const QString &host, quint16 port)
{
    if (mClient->state() == Client::Connected) {
        qDebug() << "Already connected!";
        return;
    }

    mClient->connectToServer(host, port);
}

void LoginManager::login(const QString &username, const QString &password)
{
    if (mClient->state() != Client::Connected)
        return;

//    msg.writeString(loginData->username);
//    msg.writeString(sha256(loginData->username + loginData->password));

    const QByteArray convertedUsername = username.toLatin1();
    const QByteArray convertedPassword = password.toLatin1();

    QByteArray loginMessage;
    QDataStream stream(&loginMessage, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << (quint16) PAMSG_LOGIN;
    stream << (quint32) 0;
    stream.writeRawData(convertedUsername.constData(),
                        convertedUsername.length());
    stream.writeRawData(convertedPassword.constData(),
                        convertedPassword.length());
    mClient->send(loginMessage);
}

void LoginManager::onReadyRead()
{
    qDebug() << Q_FUNC_INFO;
//    mInputBuffer += mSocket->readAll();
    // TODO: Deal with incoming bytes
}
