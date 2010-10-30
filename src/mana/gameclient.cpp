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

#include "beingmanager.h"
#include "messagein.h"
#include "messageout.h"
#include "protocol.h"

#include <iostream>

namespace Mana {

GameClient::GameClient(QObject *parent)
    : ENetClient(parent)
    , mAuthenticated(false)
    , mBeingManager(new BeingManager(this))
{
    QObject::connect(mBeingManager, SIGNAL(playerChanged()),
                     this, SIGNAL(playerChanged()));
}

GameClient::~GameClient()
{
}

BeingListModel *GameClient::beingListModel() const
{
    return mBeingManager->beingListModel();
}

Being *GameClient::player() const
{
    return mBeingManager->player();
}

QString GameClient::playerName() const
{
    return mBeingManager->playerName();
}

void GameClient::setPlayerName(const QString &name)
{
    if (mBeingManager->playerName() == name)
        return;

    mBeingManager->setPlayerName(name);
    emit playerNameChanged();
}

void GameClient::authenticate(const QString &token)
{
    // Send in the security token
    MessageOut msg(PGMSG_CONNECT);
    msg.writeString(token, 32);
    send(msg);
}

void GameClient::walkTo(int x, int y)
{
    MessageOut message(PGMSG_WALK);
    message.writeInt16(x);
    message.writeInt16(y);
    send(message);
}

void GameClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case GPMSG_CONNECT_RESPONSE:
        handleAuthenticationResponse(message);
        break;
    case GPMSG_PLAYER_MAP_CHANGE:
        handlePlayerMapChanged(message);
        break;
    case GPMSG_BEING_ENTER:
        mBeingManager->handleBeingEnter(message);
        break;
    case GPMSG_BEING_LEAVE:
        mBeingManager->handleBeingLeave(message);
        break;
    case GPMSG_BEINGS_MOVE:
        mBeingManager->handleBeingsMove(message);
        break;
    case XXMSG_INVALID:
        qWarning() << "(GameClient::messageReceived) Invalid received! "
                      "Did we send an invalid message?";
        break;
    default:
        qDebug() << "(GameClient::messageReceived) Unknown message "
                  << message;
        break;
    }
}

void GameClient::handleAuthenticationResponse(MessageIn &message)
{
    switch (message.readInt8()) {
    default:
        emit authenticationFailed(tr("Unknown error"));
        break;
    case ERRMSG_SERVER_FULL:
        emit authenticationFailed(tr("The server is full"));
        disconnect();
        break;
    case ERRMSG_OK:
        if (!mAuthenticated) {
            mAuthenticated = true;
            emit authenticatedChanged();
        }
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
