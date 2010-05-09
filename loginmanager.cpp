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
#include "messagein.h"
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

void LoginManager::connectToLoginServer(const ServerAddress &server)
{
    if (mClient->state() == Client::Connected) {
        qDebug() << "Already connected!";
        return;
    }

    mClient->connectToServer(server);
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

void LoginManager::handleMessage(const QByteArray &data)
{
    qDebug() << Q_FUNC_INFO;
    if (data.length() < 2) {
        qDebug() << "Message too short!";
        return;
    }

    MessageIn message(data);

    switch (message.id()) {
    case APMSG_LOGIN_RESPONSE:
        handleLoginResponse(message);
        break;
    case APMSG_CHAR_INFO:
        handleCharacterInfo(message);
        break;
    case XXMSG_INVALID:
        qDebug() << "Invalid received!";
        break;
    default:
        qDebug() << "Unknown message" << message.id();
        break;
    }
}

void LoginManager::handleLoginResponse(MessageIn &message)
{
    const quint8 error = message.readInt8();

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
    case LOGIN_BANNED:
        mError = tr("Account is banned");
        break;
    }

    emit loginFailed();
}

void LoginManager::handleCharacterInfo(MessageIn &message)
{
    qDebug() << Q_FUNC_INFO;
    CharacterInfo info;
    info.slot = message.readInt8();
    info.name = message.readString();
    message.readInt8(); // gender
    message.readInt8(); // hair style
    message.readInt8(); // hair color
    message.readInt16(); // level
    message.readInt16(); // character points
    message.readInt16(); // correction points
    info.money = message.readInt32();
    for (int i = 0; i < 7; i++)
    {
        message.readInt8(); // attribute
    }
    mCharacters.append(info);

    emit charactersChanged();
}
