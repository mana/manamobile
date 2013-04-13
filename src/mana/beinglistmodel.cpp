/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
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

#include "beinglistmodel.h"

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
    , mPlayerBeing(0)
{
    mRoleNames.insert(BeingRole, "being");
#if QT_VERSION < 0x050000
    setRoleNames(mRoleNames);
#endif

    mBeingUpdateTimer = startTimer(16);
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

void BeingListModel::handleBeingEnter(MessageIn &message)
{
    const int type = message.readInt8();
    const int id = message.readInt16();
    const QString &action = SpriteAction::actionByInt(message.readInt8());
    const int x = message.readInt16();
    const int y = message.readInt16();
    BeingDirection direction = (BeingDirection)message.readInt8();
    BeingGender gender = (BeingGender)message.readInt8();

    Being *being;

    if (type == OBJECT_CHARACTER) {
        Character *ch = new Character;
        ch->setName(message.readString());

        handleHair(ch, message);

        ch->setGender(gender);

        if (message.unreadLength())
            handleLooks(ch, message);

        // Match the being by name to see whether it's the current player
        if (ch->name() == mPlayerName) {
            mPlayerBeing = ch;
            emit playerChanged();
        }

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

    qDebug() << Q_FUNC_INFO << being->name() << being->id() << being->x() << being->y();
    addBeing(being);
}

void BeingListModel::handleBeingLeave(MessageIn &message)
{
    const int id = message.readInt16();

    if (mPlayerBeing && mPlayerBeing->id() == id) {
        mPlayerBeing = 0;
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
            const qreal pixelsPerSecond = (qreal) speed / 10 * 32;
            const qreal pixelsPerTimerEvent = pixelsPerSecond * 0.016;
            being->setWalkSpeed(pixelsPerTimerEvent);
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
    qDebug() << Q_FUNC_INFO;
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

void BeingListModel::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != mBeingUpdateTimer)
        return;

    for (int i = 0, end = mBeings.size(); i < end; ++i) {
        Being *being = mBeings.at(i);

        const QPointF pos = being->position();
        const QPointF target = being->serverPosition();
        if (pos == target) {
            if (being->action() == SpriteAction::WALK)
                being->setAction(SpriteAction::STAND);
            continue;
        } else {
            being->setAction(SpriteAction::WALK);
        }

        const qreal walkSpeed = being->walkSpeed();
        QVector2D direction(target - pos);

        if (direction.lengthSquared() < walkSpeed * walkSpeed) {
            being->setPosition(target);
            continue;
        }

        direction.normalize();
        direction *= walkSpeed;
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
