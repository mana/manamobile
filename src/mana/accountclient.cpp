/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *
 * This file is part of manalib.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "accountclient.h"

#include "messagein.h"
#include "messageout.h"
#include "protocol.h"
#include "sha256.h"

#include <iostream>

namespace Mana {

void AccountClient::login(const QString &username,
                          const QString &password)
{
    QByteArray combination;
    combination += username.toUtf8();
    combination += password.toUtf8();
    const QByteArray hash = sha256(combination);

    MessageOut loginMessage(PAMSG_LOGIN);
    loginMessage.writeInt32(0); // client version
    loginMessage.writeString(username);
    loginMessage.writeString(hash);
    send(loginMessage);
}

void AccountClient::chooseCharacter(const CharacterInfo &character)
{
    MessageOut m(PAMSG_CHAR_SELECT);
    m.writeInt8(character.slot);
    send(m);
}

void AccountClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case APMSG_LOGIN_RESPONSE:
        handleLoginResponse(message);
        break;
    case APMSG_CHAR_INFO:
        handleCharacterInfo(message);
        break;
    case APMSG_CHAR_SELECT_RESPONSE:
        handleCharacterSelectResponse(message);
        break;
    case XXMSG_INVALID:
        qWarning() << "(AccountClient::messageReceived) Invalid received! "
                "Did we send an invalid message?";
        break;
    default:
        qDebug() << "(AccountClient::messageReceived) Unknown message "
                << message;
        break;
    }
}

void AccountClient::handleLoginResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK) {
        mUpdateHost = message.readString();
        emit updateHostChanged();

        qDebug() << "Update host: " << mUpdateHost;

        if (message.unreadLength() > 0) {
            mDataUrl = message.readString();
            emit dataUrlChanged();

            qDebug() << "Data URL: " << mDataUrl;
        }

        emit loginSucceeded();
    } else {
        emit loginFailed(error);
    }
}

void AccountClient::handleCharacterInfo(MessageIn &message)
{
    CharacterInfo info;

    info.slot = message.readInt8();
    info.name = message.readString();
    message.readInt8(); // gender
    message.readInt8(); // hair style
    message.readInt8(); // hair color
    info.level = message.readInt16();
    message.readInt16(); // character points
    message.readInt16(); // correction points
    info.money = message.readInt32();
    for (int i = 0; i < 7; i++)
    {
        message.readInt8(); // attribute
    }

    emit characterInfoReceived(info);
}

void AccountClient::handleCharacterSelectResponse(MessageIn &message)
{
    const int error = message.readInt8();
    if (error == ERRMSG_OK) {
        // Remember chat and game server hosts
        mToken = message.readString(32);
        mGameServerHost = message.readString();
        mGameServerPort = message.readInt16();
        mChatServerHost = message.readString();
        mChatServerPort = message.readInt16();

        emit tokenChanged();
        emit gameServerHostChanged();
        emit gameServerPortChanged();
        emit chatServerHostChanged();
        emit chatServerPortChanged();

        qDebug() << "Game server: " << mGameServerHost << ":"
                << mGameServerPort;
        qDebug() << "Chat server: " << mChatServerHost << ":"
                << mChatServerPort;

        emit chooseCharacterSucceeded();
    } else {
        emit chooseCharacterFailed(error);
    }
}

} // namespace Mana
