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

#include "messagein.h"
#include "messageout.h"
#include "protocol.h"

#include <iostream>

namespace Mana {

GameClient::GameClient(QObject *parent)
    : ENetClient(parent)
{
}

void GameClient::sendToken(const QString &token)
{
    // Send in the security token
    MessageOut msg(PGMSG_CONNECT);
    msg.writeString(token, 32);
    send(msg);
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
    const QString name = message.readString();
    const int x = message.readInt16();
    const int y = message.readInt16();

    qDebug() << "Arrived at" << name << x << y;

    mCurrentMap = QLatin1String("maps/");
    mCurrentMap += name;

    const QLatin1String mapExtension(".tmx");
    if (!mCurrentMap.endsWith(mapExtension))
        mCurrentMap += mapExtension;

    emit mapChanged(mCurrentMap, x, y);
}

} // namespace Mana
