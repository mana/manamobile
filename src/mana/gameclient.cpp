/*
 * Mana QML plugin
 * Copyright (C) 2010  Thorbjørn Lindeijer
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "gameclient.h"

#include "abilitylistmodel.h"
#include "attributelistmodel.h"
#include "being.h"
#include "beinglistmodel.h"
#include "character.h"
#include "collisionhelper.h"
#include "inventorylistmodel.h"
#include "logicdriver.h"
#include "messagein.h"
#include "messageout.h"
#include "monster.h"
#include "npc.h"
#include "protocol.h"
#include "resourcemanager.h"
#include "questloglistmodel.h"

#include "resource/abilitydb.h"
#include "resource/mapresource.h"
#include "resource/spritedef.h"

#include <safeassert.h>

#include <iostream>

namespace Mana {

GameClient::GameClient(QObject *parent)
    : ENetClient(parent)
    , mAuthenticated(false)
    , mPlayerStartX(0)
    , mPlayerStartY(0)
    , mPlayerCharacter(0)
    , mNpcState(NoNpc)
    , mNpc(0)
    , mAbilityListModel(new AbilityListModel(this))
    , mAttributeListModel(new AttributeListModel(this))
    , mBeingListModel(new BeingListModel(this))
    , mInventoryListModel(new InventoryListModel(this))
    , mLogicDriver(new LogicDriver(this))
    , mQuestlogListModel(new QuestlogListModel(this))
{
    QObject::connect(mLogicDriver, SIGNAL(update(qreal)),
                     this, SLOT(update(qreal)));
}

GameClient::~GameClient()
{
}

Character *GameClient::player() const
{
    return mPlayerCharacter;
}

QPointF GameClient::playerWalkDirection() const
{
    return mPlayerWalkDirection.toPointF();
}

void GameClient::setPlayerWalkDirection(QPointF direction)
{
    const QVector2D directionV(direction);

    if (mPlayerWalkDirection != directionV) {
        mPlayerWalkDirection = directionV;

        emit playerWalkDirectionChanged();
    }
}

QString GameClient::playerName() const
{
    return mPlayerName;
}

void GameClient::setPlayerName(const QString &name)
{
    if (mPlayerName == name)
        return;

    mPlayerName = name;
    emit playerNameChanged();
}

void GameClient::authenticate(const QString &token)
{
    // Send in the security token
    MessageOut msg(Protocol::PGMSG_CONNECT);
    msg.writeString(token, 32);
    send(msg);
}

void GameClient::walkTo(int x, int y)
{
    MessageOut message(Protocol::PGMSG_WALK);
    message.writeInt16(x);
    message.writeInt16(y);
    send(message);
}

void GameClient::say(const QString &text)
{
    MessageOut message(Protocol::PGMSG_SAY);
    message.writeString(text);
    send(message);
}

void GameClient::talkToNpc(Being *being)
{
    SAFE_ASSERT(being->type() == OBJECT_NPC && !mNpc, return);

    mNpc = static_cast<NPC *>(being);
    emit npcChanged();

    MessageOut message(Protocol::PGMSG_NPC_TALK);
    message.writeInt16(mNpc->id());
    send(message);
}

void GameClient::nextNpcMessage()
{
    SAFE_ASSERT(mNpcState == NpcAwaitNext && mNpc, return);

    MessageOut message(Protocol::PGMSG_NPC_TALK_NEXT);
    message.writeInt16(mNpc->id());
    send(message);
}

void GameClient::chooseNpcOption(int choice)
{
    SAFE_ASSERT(mNpcState == NpcAwaitChoice && mNpc, return);

    MessageOut message(Protocol::PGMSG_NPC_SELECT);
    message.writeInt16(mNpc->id());
    message.writeInt8(choice);
    send(message);
}

void GameClient::useAbilityOnPoint(unsigned id, int x, int y)
{
    MessageOut message(Protocol::PGMSG_USE_ABILITY_ON_POINT);
    message.writeInt8(id);
    message.writeInt16(x);
    message.writeInt16(y);
    send(message);
}

void GameClient::useAbilityOnDirection(unsigned id)
{
    MessageOut message(Protocol::PGMSG_USE_ABILITY_ON_DIRECTION);
    message.writeInt8(id);
    message.writeInt8(mPlayerCharacter->direction());
    send(message);
}

void GameClient::equip(unsigned slot)
{
    MessageOut message(Protocol::PGMSG_EQUIP);
    message.writeInt16(slot);
    send(message);
}

void GameClient::unequip(unsigned slot)
{
    MessageOut message(Protocol::PGMSG_UNEQUIP);
    message.writeInt16(slot);
    send(message);
}

void GameClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case Protocol::GPMSG_CONNECT_RESPONSE:
        handleAuthenticationResponse(message);
        break;
    case Protocol::GPMSG_PLAYER_MAP_CHANGE:
        handlePlayerMapChanged(message);
        break;

    case Protocol::GPMSG_INVENTORY:
        handleInventory(message);
        break;
    case Protocol::GPMSG_INVENTORY_FULL:
        handleInventoryFull(message);
        break;
    case Protocol::GPMSG_EQUIP:
        handleEquip(message);
        break;
    case Protocol::GPMSG_UNEQUIP:
        handleUnEquip(message);
        break;

    case Protocol::GPMSG_PLAYER_ATTRIBUTE_CHANGE:
        handlePlayerAttributeChange(message);
        break;

    case Protocol::GPMSG_BEING_ENTER:
        handleBeingEnter(message);
        break;
    case Protocol::GPMSG_BEING_LEAVE:
        handleBeingLeave(message);
        break;
    case Protocol::GPMSG_BEING_LOOKS_CHANGE:
        handleBeingLooksChange(message);
        break;
    case Protocol::GPMSG_BEING_ACTION_CHANGE:
        handleBeingActionChange(message);
        break;
    case Protocol::GPMSG_BEING_DIR_CHANGE:
        handleBeingDirChange(message);
        break;
    case Protocol::GPMSG_BEINGS_MOVE:
        handleBeingsMove(message);
        break;
    case Protocol::GPMSG_BEING_ABILITY_POINT:
        handleBeingAbilityOnPoint(message);
        break;
    case Protocol::GPMSG_BEING_ABILITY_BEING:
        handleBeingAbilityOnBeing(message);
        break;
    case Protocol::GPMSG_BEING_ABILITY_DIRECTION:
        handleBeingAbilityOnDirection(message);
        break;
    case Protocol::GPMSG_ABILITY_STATUS:
        handleAbilityStatus(message);
        break;
    case Protocol::GPMSG_ABILITY_REMOVED:
        handleAbilityRemoved(message);
        break;
    case Protocol::GPMSG_SAY:
        handleBeingSay(message);
        break;

    case Protocol::GPMSG_NPC_CHOICE:
        handleNpcChoice(message);
        break;
    case Protocol::GPMSG_NPC_MESSAGE:
        handleNpcMessage(message);
        break;
    case Protocol::GPMSG_NPC_CLOSE:
        handleNpcClose(message);
        break;

    case Protocol::GPMSG_BEINGS_DAMAGE:
        handleBeingsDamage(message);
        break;

    case Protocol::GPMSG_QUESTLOG_STATUS:
        handleQuestlogStatus(message);
        break;

    case Protocol::XXMSG_INVALID:
        qWarning() << "(GameClient::messageReceived) Invalid received! "
                      "Did we send an invalid message?";
        break;
    default:
        qDebug() << "(GameClient::messageReceived) Unknown message "
                  << message;
        break;
    }
}

void GameClient::update(qreal deltaTime)
{
    foreach (Being *being, mBeingListModel->beings()) {
        if (being == mPlayerCharacter)
            continue;

        const QPointF pos = being->position();
        const QPointF target = being->serverPosition();

        if (pos == target) {
            if (being->action() == SpriteAction::WALK)
                being->setAction(SpriteAction::STAND);
            continue;
        } else {
            being->setAction(SpriteAction::WALK);
        }

        const qreal walkDistance = being->walkSpeed() * deltaTime;
        QVector2D direction(target - pos);

        if (direction.lengthSquared() < walkDistance * walkDistance) {
            being->setPosition(target);
            continue;
        }

        direction.normalize();
        direction *= walkDistance;
        QPointF newPos = pos + direction.toPointF();
        being->lookAt(newPos);
        being->setPosition(newPos);
    }

    if (mPlayerCharacter)
        updatePlayer(deltaTime);
}

void GameClient::updatePlayer(qreal deltaTime)
{
    const Tiled::TileLayer *collisionLayer = mMapResource->collisionLayer();
    if (!collisionLayer)
        return;

    const qreal walkDistance = mPlayerCharacter->walkSpeed() * deltaTime;
    QVector2D direction = mPlayerWalkDirection;

    if (direction.isNull() || !walkDistance) {
        if (mPlayerCharacter->action() == SpriteAction::WALK)
            mPlayerCharacter->setAction(SpriteAction::STAND);
        return;
    }

    direction.normalize();
    direction *= walkDistance;

    const QPointF pos = mPlayerCharacter->position();

    // The radius is smaller than half a tile to make narrow passages usable
    CollisionHelper collisionHelper(collisionLayer);
    QPointF newPos = collisionHelper.adjustMove(pos, direction.toPointF(), 14);

    if (newPos == pos) {
        // Player is not allowed to walk, but direction should still change
        mPlayerCharacter->lookAt(pos + mPlayerWalkDirection.toPointF());
        if (mPlayerCharacter->action() == SpriteAction::WALK)
            mPlayerCharacter->setAction(SpriteAction::STAND);
        return;
    }

    // Try to move straight at maximum speed if the player could not move in
    // the intended non-straight direction.
    if (direction.x() != 0 && direction.y() != 0) {
        if (newPos.x() == pos.x())
            direction.setY(direction.y() < 0 ? -walkDistance : walkDistance);
        else if (newPos.y() == pos.y())
            direction.setX(direction.x() < 0 ? -walkDistance : walkDistance);

        newPos = collisionHelper.adjustMove(pos, direction.toPointF(), 14);
    }

    mPlayerCharacter->lookAt(newPos);
    mPlayerCharacter->setPosition(newPos);

    playerPositionChanged();
    mPlayerCharacter->setAction(SpriteAction::WALK);
}

void GameClient::playerPositionChanged()
{
    // TODO: Rate-limit these calls
    const Being *ch = player();
    walkTo(ch->x(), ch->y());
}

void GameClient::restoreWalkingSpeed()
{
    const AttributeValue *attribute =
            mAttributeListModel->attribute(ATTR_MOVE_SPEED_TPS);
    if (attribute && mPlayerCharacter) {
        qreal speed = AttributeListModel::tpsToPixelsPerSecond(attribute->modified());
        mPlayerCharacter->setWalkSpeed(speed);
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
    case ERRMSG_ADMINISTRATIVE_LOGOFF:
        emit kicked();
        break;
    case ERRMSG_OK:
        if (!mAuthenticated) {
            mAuthenticated = true;
            mLogicDriver->start(); // start driving logic
            emit authenticatedChanged();
        }
        break;
    }
}

void GameClient::handlePlayerMapChanged(MessageIn &message)
{
    mCurrentMap = message.readString();
    mPlayerStartX = message.readInt16();
    mPlayerStartY = message.readInt16();

    if (mMapResource)
        mMapResource->decRef();

    mMapResource = ResourceManager::instance()->requestMap(mCurrentMap);

    // Reset the player being before it gets deleted
    if (mPlayerCharacter) {
        mPlayerCharacter = 0;
        emit playerChanged();
    }

    // None of the beings are valid on the new map, including the player
    mBeingListModel->clear();

    emit mapChanged(mCurrentMap, mPlayerStartX, mPlayerStartY);
}

void GameClient::handleInventory(MessageIn &message)
{
    while (message.unreadData()) {
        unsigned slot = message.readInt16();
        int id = message.readInt16(); // 0 id means removal of slot
        unsigned amount = id ? message.readInt16() : 0;

        mInventoryListModel->setItemSlot(slot, id, amount, 0);
    }
}

void GameClient::handleInventoryFull(MessageIn &message)
{
    mInventoryListModel->removeAllItems();

    int count = message.readInt16();
    while (count--) {
        int slot = message.readInt16();
        int id = message.readInt16();
        int amount = message.readInt16();
        int equipmentSlot = message.readInt16();
        mInventoryListModel->setItemSlot(slot, id, amount, equipmentSlot);
    }
}

void GameClient::handleEquip(MessageIn &message)
{
    const unsigned inventorySlot = message.readInt16();
    const unsigned equipmentSlot = message.readInt16();
    mInventoryListModel->equip(inventorySlot, equipmentSlot);
}

void GameClient::handleUnEquip(MessageIn &message)
{
    const unsigned slot = message.readInt16();
    mInventoryListModel->unequip(slot);
}

void GameClient::handlePlayerAttributeChange(MessageIn &message)
{
    while (message.unreadData()) {
        const int id = message.readInt16();
        const qreal base = message.readInt32() / 256;
        const qreal mod = message.readInt32() / 256;

        if (id == ATTR_MOVE_SPEED_TPS)
            player()->setWalkSpeed(AttributeListModel::tpsToPixelsPerSecond(base));

        mAttributeListModel->setAttribute(id, base, mod);
    }
}

static void handleHair(Character *ch, MessageIn &message)
{
    int hairstyle = message.readInt8();
    int haircolor = message.readInt8();

    ch->setHairStyle(hairstyle, haircolor);
}

static void handleLooks(Character *ch, MessageIn &message)
{
    handleHair(ch, message);

    int numberOfChanges = message.unreadData() ? message.readInt8() : 0;
    QMap<int, int> equippedSlots;

    while (numberOfChanges-- > 0) {
        int slot = message.readInt8();
        int itemId = message.readInt16();

        equippedSlots[slot] = itemId;
    }
    ch->setEquipmentSlots(equippedSlots);
}

void GameClient::handleBeingEnter(MessageIn &message)
{
    const int type = message.readInt8();
    const int id = message.readInt16();
    const QString &action = SpriteAction::actionByInt(message.readInt8());
    const int x = message.readInt16();
    const int y = message.readInt16();
    BeingDirection direction = static_cast<BeingDirection>(message.readInt8());
    Being::BeingGender gender = static_cast<Being::BeingGender>(message.readInt8());

    Being *being;
    Character *playerCharacter = 0;

    if (type == OBJECT_CHARACTER) {
        Character *ch = new Character;
        QString name = message.readString();

        ch->setName(name);
        ch->setGender(gender);

        handleLooks(ch, message);

        // Match the being by name to see whether it's the current player
        if (ch->name() == mPlayerName)
            playerCharacter = ch;

        being = ch;
    } else if (type == OBJECT_NPC) {
        int spriteId = message.readInt16();
        QString name = message.readString();

        NPC *npc = new NPC(spriteId);
        npc->setName(name);
        npc->setGender(gender);

        being = npc;
    } else if (type == OBJECT_MONSTER) {
        int monsterId = message.readInt16();
        QString name = message.readString();

        Monster *monster = new Monster(monsterId);
        monster->setName(name);
        monster->setGender(gender);

        being = monster;
    } else {
        return;
    }

    being->setId(id);
    being->setPosition(QPointF(x, y));
    being->setServerPosition(QPointF(x, y));
    being->setAction(action);
    being->setDirection(direction);

    mBeingListModel->addBeing(being);

    // Emit playerChanged after the player has been fully initialized and added
    if (playerCharacter) {
        mPlayerCharacter = playerCharacter;
        restoreWalkingSpeed();
        emit playerChanged();
    }
}

void GameClient::handleBeingLeave(MessageIn &message)
{
    const int id = message.readInt16();

    if (mPlayerCharacter && mPlayerCharacter->id() == id) {
        mPlayerCharacter = 0;
        emit playerChanged();
    }

    mBeingListModel->removeBeing(id);
}

void GameClient::handleBeingLooksChange(MessageIn &message)
{
    const int id = message.readInt16();

    Being *being = mBeingListModel->beingById(id);

    if (being) {
        SAFE_ASSERT(being->type() == OBJECT_CHARACTER, return);
        Character *ch = static_cast<Character *>(being);
        handleLooks(ch, message);
    }
}

void GameClient::handleBeingActionChange(MessageIn &message)
{
    const int id = message.readInt16();

    Being *being = mBeingListModel->beingById(id);

    if (being) {
        int actionAsInt = message.readInt8();
        const QString &newAction = SpriteAction::actionByInt(actionAsInt);

        if (newAction == SpriteAction::STAND &&
                being->action() == SpriteAction::WALK)
            return; // Client knows when to stop movement
        being->setAction(newAction);
    }
}

void GameClient::handleBeingDirChange(MessageIn &message)
{
    const int id = message.readInt16();
    const BeingDirection dir = static_cast<BeingDirection>(message.readInt8());

    if (Being *being = mBeingListModel->beingById(id))
        being->setDirection(dir);
}

void GameClient::handleBeingsMove(MessageIn &message)
{
    while (message.unreadData()) {
        const int id = message.readInt16();
        const int flags = message.readInt8();

        if (!(flags & (MOVING_POSITION | MOVING_DESTINATION)))
            continue;

        int dx = 0, dy = 0, speed = 0;

        if (flags & MOVING_POSITION) {
            message.readInt16(); // unused previous x position
            message.readInt16(); // unused previous y position
        }

        if (flags & MOVING_DESTINATION) {
            dx = message.readInt16();
            dy = message.readInt16();
            speed = message.readInt8();
        }

        Being *being = mBeingListModel->beingById(id);
        if (!being)
            continue;

        if (speed) {
            /*
             * The being's speed is transfered in tiles per second * 10
             * to keep it transferable in a byte.
             */
            const qreal tps = (qreal) speed / 10;
            being->setWalkSpeed(AttributeListModel::tpsToPixelsPerSecond(tps));
        }

        if (flags & MOVING_DESTINATION) {
            QPointF pos(dx, dy);
            being->setServerPosition(pos);
        }
    }
}

void GameClient::handleBeingAbilityOnPoint(MessageIn &message)
{
    const int id = message.readInt16();
    const int abilityId = message.readInt8();
    const int x = message.readInt16();
    const int y = message.readInt16();

    Being *being = mBeingListModel->beingById(id);

    if (being) {
        being->lookAt(QPointF(x, y));
        const AbilityInfo *abilityInfo =
                AbilityDB::instance()->getInfo(abilityId);
        if (!abilityInfo) {
            qWarning() << Q_FUNC_INFO << "The server sent unknown ability"
                       << " as being used. Mismatching world data?";
            return;
        }
        being->setAction(abilityInfo->useAction());
    }
}

void GameClient::handleBeingAbilityOnBeing(MessageIn &message)
{
    const int id = message.readInt16();
    const int abilityId = message.readInt8();
    const int otherBeingId = message.readInt16();

    Being *being = mBeingListModel->beingById(id);
    Being *otherBeing = mBeingListModel->beingById(otherBeingId);

    if (being && otherBeing) {
        being->lookAt(otherBeing->position());
        const AbilityInfo *abilityInfo =
                AbilityDB::instance()->getInfo(abilityId);
        if (!abilityInfo) {
            qWarning() << Q_FUNC_INFO << "The server sent unknown ability"
                       << " as being used. Mismatching world data?";
            return;
        }
        being->setAction(abilityInfo->useAction());
    }
}

void GameClient::handleBeingAbilityOnDirection(MessageIn &message)
{
    const int id = message.readInt16();
    const int abilityId = message.readInt8();
    BeingDirection direction = (BeingDirection)message.readInt8();

    Being *being = mBeingListModel->beingById(id);

    if (being) {
        being->setDirection(direction);
        const AbilityInfo *abilityInfo =
                AbilityDB::instance()->getInfo(abilityId);
        if (!abilityInfo) {
            qWarning() << Q_FUNC_INFO << "The server sent unknown ability"
                       << " as being used. Mismatching world data?";
            return;
        }
        being->setAction(abilityInfo->useAction());
    }
}

void GameClient::handleAbilityStatus(MessageIn &messageIn)
{
    while (messageIn.unreadData()) {
        unsigned id = messageIn.readInt8();
        unsigned remainingTicks = messageIn.readInt32();
        mAbilityListModel->setAbilityStatus(id, remainingTicks * 100);
    }
}

void GameClient::handleAbilityRemoved(MessageIn &messageIn)
{
    unsigned id = messageIn.readInt8();
    mAbilityListModel->takeAbility(id);
}

void GameClient::handleBeingSay(MessageIn &message)
{
    const int id = message.readInt16();
    const QString text = message.readString();

    if (id) {
        if (Being *being = mBeingListModel->beingById(id)) {
            emit being->chatMessage(text);
            emit chatMessage(being, text);
        }
    } else {
        emit chatMessage(0, text); // message from server
    }
}

void GameClient::handleNpcChoice(MessageIn &message)
{
    mNpcChoices.clear();

    int id = message.readInt16();

    while (message.unreadData())
        mNpcChoices.append(message.readString());

    mNpcState = NpcAwaitChoice;
    emit npcChoicesChanged();
    emit npcStateChanged();
}

void GameClient::handleNpcMessage(MessageIn &message)
{
    int id = message.readInt16();
    QString text = message.readString();

    mNpcMessage = text;
    mNpcState = NpcAwaitNext;
    emit npcMessageChanged();
    emit npcStateChanged();
}

void GameClient::handleNpcClose(MessageIn &message)
{
    int id = message.readInt16();

    mNpcState = NoNpc;
    mNpc = 0;
    emit npcStateChanged();
    emit npcChanged();
}

void GameClient::handleBeingsDamage(MessageIn &message)
{
    while (message.unreadData()) {
        const int beingId = message.readInt16();
        const int amount = message.readInt16();

        if (Being *being = mBeingListModel->beingById(beingId))
            emit being->damageTaken(amount);
    }
}

void GameClient::handleQuestlogStatus(MessageIn &message)
{
    int id = message.readInt16();
    int flags = message.readInt8();
    Quest *quest = questlogListModel()->createOrGetQuest(id);
    if (flags & QUESTLOG_UPDATE_STATE)
        quest->setState((Quest::State)message.readInt8());
    if (flags & QUESTLOG_UPDATE_TITLE)
        quest->setTitle(message.readString());
    if (flags & QUESTLOG_UPDATE_DESCRIPTION)
        quest->setDescription(message.readString());
}

} // namespace Mana
