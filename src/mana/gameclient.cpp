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

#include "attributelistmodel.h"
#include "being.h"
#include "beinglistmodel.h"
#include "character.h"
#include "inventorylistmodel.h"
#include "messagein.h"
#include "messageout.h"
#include "npcdialogmanager.h"
#include "protocol.h"

#include <iostream>

namespace Mana {

GameClient::GameClient(QObject *parent)
    : ENetClient(parent)
    , mAuthenticated(false)
    , mAttributeListModel(new AttributeListModel(this))
    , mBeingListModel(new BeingListModel(this))
    , mNpcDialogManager(new NpcDialogManager(this))
    , mInventoryListModel(new InventoryListModel(this))
{
    QObject::connect(mBeingListModel, SIGNAL(playerChanged()),
                     this, SIGNAL(playerChanged()));
    QObject::connect(mBeingListModel, SIGNAL(playerChanged()),
                     this, SLOT(restoreWalkingSpeed()));


    QObject::connect(mNpcDialogManager, SIGNAL(startTalking(int)),
                     this, SLOT(startedTalkingToNpc(int)));
    QObject::connect(mNpcDialogManager, SIGNAL(nextMessage(int)),
                     this, SLOT(nextNpcTalk(int)));
    QObject::connect(mNpcDialogManager, SIGNAL(doChoice(int,int)),
                     this, SLOT(doNpcChoice(int,int)));

    QObject::connect(mBeingListModel, SIGNAL(playerWalkDirectionChanged()),
                     this, SIGNAL(playerWalkDirectionChanged()));
    QObject::connect(mBeingListModel, SIGNAL(playerPositionChanged()),
                     this, SLOT(playerPositionChanged()));
    QObject::connect(mBeingListModel, SIGNAL(playerChanged()),
                     this, SIGNAL(playerChanged()));
}

GameClient::~GameClient()
{
}

BeingListModel *GameClient::beingListModel() const
{
    return mBeingListModel;
}

Character *GameClient::player() const
{
    return mBeingListModel->player();
}

InventoryListModel *GameClient::inventoryListModel() const
{
    return mInventoryListModel;
}


QPointF GameClient::playerWalkDirection() const
{
    return mBeingListModel->playerWalkDirection().toPointF();
}

void GameClient::setPlayerWalkDirection(QPointF direction)
{
    mBeingListModel->setPlayerWalkDirection(QVector2D(direction));
}

QString GameClient::playerName() const
{
    return mBeingListModel->playerName();
}

void GameClient::setPlayerName(const QString &name)
{
    if (mBeingListModel->playerName() == name)
        return;

    mBeingListModel->setPlayerName(name);
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

void GameClient::say(const QString &text)
{
    MessageOut message(PGMSG_SAY);
    message.writeString(text);
    send(message);
}

void GameClient::startedTalkingToNpc(int npcId)
{
    MessageOut message(PGMSG_NPC_TALK);
    message.writeInt16(npcId);
    send(message);
}

void GameClient::nextNpcTalk(int npcId)
{
    MessageOut message(PGMSG_NPC_TALK_NEXT);
    message.writeInt16(npcId);
    send(message);
}

void GameClient::doNpcChoice(int npcId, int choice)
{
    MessageOut message(PGMSG_NPC_SELECT);
    message.writeInt16(npcId);
    message.writeInt8(choice);
    send(message);
}

void GameClient::restoreWalkingSpeed()
{
    const AttributeValue *attribute =
            mAttributeListModel->attribute(ATTR_MOVE_SPEED_TPS);
    if (attribute && player()) {
        qreal speed = AttributeListModel::tpsToPixelsPerSecond(attribute->modified());
        player()->setWalkSpeed(speed);
    }
}

void GameClient::equip(unsigned slot)
{
    MessageOut message(PGMSG_EQUIP);
    message.writeInt16(slot);
    send(message);
}

void GameClient::unequip(unsigned slot)
{
    MessageOut message(PGMSG_UNEQUIP);
    message.writeInt16(slot);
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

    case GPMSG_INVENTORY:
        mInventoryListModel->handleInventory(message);
        break;
    case GPMSG_INVENTORY_FULL:
        mInventoryListModel->handleInventoryFull(message);
        break;
    case GPMSG_EQUIP:
        mInventoryListModel->handleEquip(message);
        break;
    case GPMSG_UNEQUIP:
        mInventoryListModel->handleUnEquip(message);
        break;

    case GPMSG_BEING_ENTER:
        mBeingListModel->handleBeingEnter(message);
        break;
    case GPMSG_BEING_LEAVE:
        mBeingListModel->handleBeingLeave(message);
        break;
    case GPMSG_BEING_DIR_CHANGE:
        mBeingListModel->handleBeingDirChange(message);
        break;
    case GPMSG_BEINGS_MOVE:
        mBeingListModel->handleBeingsMove(message);
        break;
    case GPMSG_BEING_LOOKS_CHANGE:
        mBeingListModel->handleBeingLooksChange(message);
        break;
    case GPMSG_BEING_ACTION_CHANGE:
        mBeingListModel->handleBeingActionChange(message);
        break;
    case GPMSG_SAY:
        mBeingListModel->handleBeingSay(message);
        break;

    case GPMSG_NPC_MESSAGE:
        mNpcDialogManager->handleNpcMessage(message);
        break;
    case GPMSG_NPC_CHOICE:
        mNpcDialogManager->handleNpcChoice(message);
        break;
    case GPMSG_NPC_CLOSE:
        mNpcDialogManager->handleNpcClose(message);
        break;

    case GPMSG_PLAYER_ATTRIBUTE_CHANGE:
        handlePlayerAttributeChange(message);
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

void GameClient::playerPositionChanged()
{
    const Being *ch = player();
    walkTo(ch->x(), ch->y());
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
    case ERRMSG_ADMINISTRATIVE_LOGOFF:
        emit kicked();
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

    // None of the beings are valid on the new map, including the player
    mBeingListModel->clear();
}

void GameClient::handlePlayerAttributeChange(MessageIn &message)
{
    while (message.unreadLength()) {
        const int id = message.readInt16();
        const qreal base = message.readInt32() / 256;
        const qreal mod = message.readInt32() / 256;

        if (id == ATTR_MOVE_SPEED_TPS)
            player()->setWalkSpeed(AttributeListModel::tpsToPixelsPerSecond(base));

        mAttributeListModel->setAttribute(id, base, mod);
    }
}

} // namespace Mana
