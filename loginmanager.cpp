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

#include "loginmanager.h"

#include "client.h"
#include "messageout.h"
#include "protocol.h"
#include "sha256.h"

#include <QIODevice>
#include <QDebug>

LoginManager::LoginManager(QObject *parent) :
    QObject(parent),
    mClient(new Client(this))
{
    connect(mClient, SIGNAL(connected()), SIGNAL(connected()));
    connect(mClient, SIGNAL(disconnected()), SIGNAL(disconnected()));
    connect(mClient, SIGNAL(messageReceived(QByteArray)),
            SLOT(handleMessage(QByteArray)));
}

void LoginManager::connectToLoginServer(const QString &host, quint16 port)
{
    if (mClient->state() == Client::Connected) {
        qDebug() << "Already connected!";
        return;
    }

    mClient->connectToServer(host, port);
}

void LoginManager::disconnectFromLoginServer()
{
    mClient->disconnectFromServer();
}

bool LoginManager::isConnected() const
{
    return mClient->state() == Client::Connected;
}

void LoginManager::login(const QString &username, const QString &password)
{
    if (mClient->state() != Client::Connected)
        return;

    const QByteArray convertedUsername = username.toLatin1();
    const QByteArray convertedPassword = password.toLatin1();
    const QByteArray hashedPassword = sha256(convertedUsername
                                             + convertedPassword);

    MessageOut loginMessage(PAMSG_LOGIN);
    loginMessage.writeInt32(0); // client version
    loginMessage.writeString(convertedUsername);
    loginMessage.writeString(hashedPassword);
    mClient->send(loginMessage);
}

void LoginManager::handleMessage(const QByteArray &message)
{
    qDebug() << Q_FUNC_INFO;
    if (message.length() < 2) {
        qDebug() << "Message too short!";
        return;
    }

    QDataStream stream(message);
    quint16 id;
    stream >> id;

    switch (id) {
    case APMSG_LOGIN_RESPONSE: {
            quint8 error;
            stream >> error;

            switch (error) {
            case ERRMSG_OK:
                emit loginSucceeded();
                return;
            case ERRMSG_FAILURE:
                mError = tr("Unknown error");
                break;
            case ERRMSG_INVALID_ARGUMENT:
                mError = tr("Wrong user name or password");
                break;
            case LOGIN_INVALID_TIME:
                mError = tr("Tried to login too fast");
                break;
            case LOGIN_INVALID_VERSION:
                mError = tr("Client version too old");
                break;
            case LOGIN_SERVER_FULL:
                mError = tr("Account is banned");
                break;
            }

            emit loginFailed();
        }
        break;
    case XXMSG_INVALID:
        qDebug() << "Invalid received!";
        break;
    default:
        qDebug() << "Unknown message" << id;
        break;
    }
}
