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

#include "gameclient.h"

#include <mana/gamehandlerinterface.h>
#include <mana/manaclient.h>
#include <mana/protocol.h>

#include "messagein.h"
#include "messageout.h"

#include <iostream>

namespace Mana {
namespace Internal {

GameClient::GameClient(ManaClient *manaClient)
    : mGameHandler(0)
    , mManaClient(manaClient)
{
}

void GameClient::connected()
{
    std::cout << "(GameClient::connected)" << std::endl;
    mGameHandler->connected();

    // Send in the security token
    MessageOut msg(PGMSG_CONNECT);
    msg.writeString(mManaClient->token(), 32);
    send(msg);
}

void GameClient::disconnected()
{
    std::cout << "(GameClient::disconnected)" << std::endl;
    mGameHandler->disconnected();
}

void GameClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case GPMSG_PLAYER_MAP_CHANGE:
        handlePlayerMapChanged(message);
        break;
    case XXMSG_INVALID:
        std::cerr << "(GameClient::messageReceived) Invalid received! "
                "Did we send an invalid message?" << std::endl;
        break;
    default:
        std::cout << "(GameClient::messageReceived) Unknown message "
                << message << std::endl;
        break;
    }
}

void GameClient::handlePlayerMapChanged(MessageIn &message)
{
    const std::string name = message.readString();
    const int x = message.readInt16();
    const int y = message.readInt16();

    mGameHandler->mapChanged(name, x, y);
}

} // namespace Internal
} // namespace Mana
