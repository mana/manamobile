/*
 * Mana QML plugin
 * Copyright (C) 2010  Thorbjørn Lindeijer
 * Copyright (C) 2012  Erik Schilling
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

#include "beinglistmodel.h"

#include "attributelistmodel.h"
#include "being.h"
#include "character.h"
#include "messagein.h"
#include "monster.h"
#include "npc.h"
#include "protocol.h"

#include "resource/hairdb.h"
#include "resource/spritedef.h"

#include <safeassert.h>

#include <QTimerEvent>
#include <QVector2D>
#include <QDebug>

using namespace Mana;

BeingListModel::BeingListModel(QObject *parent)
    : QAbstractListModel(parent)
    , mPlayerCharacter(0)
{
    mRoleNames.insert(BeingRole, "being");

    mBeingUpdateTimer = startTimer(16);
    mFrameDurationTimer.start();
}

int BeingListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mBeings.size();
}

QVariant BeingListModel::data(const QModelIndex &index, int role) const
{
    if (role == BeingRole) {
        Being *being = mBeings.at(index.row());
        return QVariant::fromValue(being);
    }

    return QVariant();
}

QHash<int, QByteArray> BeingListModel::roleNames() const
{
    return mRoleNames;
}

void BeingListModel::setPlayerWalkDirection(QVector2D direction)
{
    if (mPlayerWalkDirection != direction) {
        mPlayerWalkDirection = direction;

        emit playerWalkDirectionChanged();
    }
}

void BeingListModel::handleBeingEnter(MessageIn &message)
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

        handleHair(ch, message);

        if (message.unreadLength())
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

    addBeing(being);

    // Emit playerChanged after the player has been fully initialized and added
    if (playerCharacter) {
        mPlayerCharacter = playerCharacter;
        emit playerChanged();
    }
}

void BeingListModel::handleBeingLeave(MessageIn &message)
{
    const int id = message.readInt16();

    if (mPlayerCharacter && mPlayerCharacter->id() == id) {
        mPlayerCharacter = 0;
        emit playerChanged();
    }

    removeBeing(id);
}

void BeingListModel::handleBeingDirChange(MessageIn &message)
{
    const int id = message.readInt16();
    const int index = indexOfBeing(id);
    BeingDirection dir = (BeingDirection)message.readInt8();
    SAFE_ASSERT(index != -1, return);

    Being *being = mBeings.at(index);
    being->setDirection(dir);
}

void BeingListModel::handleBeingsMove(MessageIn &message)
{
    while (message.unreadLength() > 0) {
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

        Being *being = beingById(id);
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

void BeingListModel::handleLooks(Character *ch, MessageIn &message)
{
    int numberOfChanges = message.readInt8();

    while (numberOfChanges-- > 0) {
        int slot = message.readInt8();
        int itemId = message.readInt16();

        ch->setEquipmentSlot(slot, itemId);
    }
}

void BeingListModel::handleHair(Character *ch, MessageIn &message)
{
    int hairstyle = message.readInt8();
    int haircolor = message.readInt8();

    ch->setHairStyle(hairstyle, haircolor);
}

void BeingListModel::handleBeingLooksChange(MessageIn &message)
{
    const int id = message.readInt16();
    if (Being *being = beingById(id)) {
        SAFE_ASSERT(being->type() == OBJECT_CHARACTER, return);
        Character *ch = static_cast<Character *>(being);
        handleLooks(ch, message);

        // Further data is hair (if available)
        if (message.unreadLength())
            handleHair(ch, message);
    }
}

void BeingListModel::handleBeingActionChange(MessageIn &message)
{
    const int id = message.readInt16();
    if (Being *being = beingById(id)) {
        int actionAsInt = message.readInt8();
        const QString &newAction = SpriteAction::actionByInt(actionAsInt);

        if (newAction == SpriteAction::STAND &&
                being->action() == SpriteAction::WALK)
            return; // Client knows when to stop movement
        being->setAction(newAction);
    }
}

void BeingListModel::handleBeingSay(MessageIn &message)
{
    const int id = message.readInt16();
    const int index = indexOfBeing(id);

    if (index != -1) {
        const QString text = message.readString();
        mBeings.at(index)->say(text);
    }
}

void BeingListModel::clear()
{
    // Reset the player being before it gets deleted
    if (mPlayerCharacter) {
        mPlayerCharacter = 0;
        emit playerChanged();
    }

    // Remove all beings from the model
    if (!mBeings.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, mBeings.size() - 1);
        qDeleteAll(mBeings);
        mBeings.clear();
        endRemoveRows();
    }
}

void BeingListModel::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != mBeingUpdateTimer)
        return;

    // Don't jump ahead more than 1 second
    qreal dt = qMin(qreal(1), mFrameDurationTimer.restart() / qreal(1000));

    for (int i = 0, end = mBeings.size(); i < end; ++i) {
        Being *being = mBeings.at(i);

        const QPointF pos = being->position();
        const qreal walkDistance = being->walkSpeed() * dt;

        if (being == mPlayerCharacter) {
            QVector2D direction = mPlayerWalkDirection;

            if (direction.isNull() || !walkDistance) {
                being->setAction(SpriteAction::STAND);
                continue;
            }

            direction.normalize();
            direction *= walkDistance;
            QPointF newPos(pos.x() + direction.x(), pos.y() + direction.y());
            being->lookAt(newPos);
            being->setPosition(newPos);

            emit playerPositionChanged();
            being->setAction(SpriteAction::WALK);
            continue;
        }

        const QPointF target = being->serverPosition();
        if (pos == target) {
            if (being->action() == SpriteAction::WALK)
                being->setAction(SpriteAction::STAND);
            continue;
        } else {
            being->setAction(SpriteAction::WALK);
        }

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
}

Being *BeingListModel::beingById(int id) const
{
    int index = indexOfBeing(id);
    if (index != -1)
        return beingAt(index);
    return 0;
}

void BeingListModel::addBeing(Being *being)
{
    beginInsertRows(QModelIndex(), mBeings.size(), mBeings.size());
    mBeings.append(being);
    endInsertRows();
}

void BeingListModel::removeBeing(int id)
{
    const int index = indexOfBeing(id);
    SAFE_ASSERT(index != -1, return);

    beginRemoveRows(QModelIndex(), index, index);
    delete mBeings.takeAt(index);
    endRemoveRows();
}

int BeingListModel::indexOfBeing(int id) const
{
    // TODO: Would be better to have a map/hash
    for (int i = 0, end = mBeings.size(); i < end; i++) {
        Being *being = mBeings.at(i);
        if (being->id() == id)
            return i;
    }
    return -1;
}
