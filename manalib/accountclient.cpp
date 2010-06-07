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

#include <mana/accounthandlerinterface.h>
#include <mana/protocol.h>

#include "messagein.h"

#include <iostream>

namespace Mana {
namespace Internal {

void AccountClient::connected()
{
    mAccountHandler->connected();
}

void AccountClient::disconnected()
{
    mAccountHandler->disconnected();
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
        std::cerr << "(AccountClient::messageReceived) Invalid received! "
                "Did we send an invalid message?" << std::endl;
        break;
    default:
        std::cout << "(AccountClient::messageReceived) Unknown message "
                << message << std::endl;
        break;
    }
}

void AccountClient::handleLoginResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK) {
        mUpdateHost = message.readString();
        std::cout << "Update host: " << mUpdateHost << std::endl;

        if (message.unreadLength() > 0) {
            mDataUrl = message.readString();
            std::cout << "Data URL: " << mDataUrl << std::endl;
        }

        mAccountHandler->loginSucceeded();
    } else {
        mAccountHandler->loginFailed(error);
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

    mAccountHandler->characterInfoReceived(info);
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

        std::cout << "Game server: " << mGameServerHost.c_str() << ":"
                << mGameServerPort << std::endl;
        std::cout << "Chat server: " << mChatServerHost.c_str() << ":"
                << mChatServerPort << std::endl;

        mAccountHandler->chooseCharacterSucceeded();
    } else {
        mAccountHandler->chooseCharacterFailed(error);
    }
}

} // namespace Internal
} // namespace Mana
