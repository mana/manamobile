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

#include "beinglistmodel.h"

#include "being.h"
#include "messagein.h"
#include "protocol.h"

#include <QTimerEvent>
#include <QVector2D>
#include <QDebug>

using namespace Mana;

BeingListModel::BeingListModel(QObject *parent)
    : QAbstractListModel(parent)
    , mPlayerBeing(0)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(BeingName, "name");
    roleNames.insert(BeingChatMessage, "chatMessage");
    roleNames.insert(BeingX, "x");
    roleNames.insert(BeingY, "y");
    setRoleNames(roleNames);

    mBeingUpdateTimer = startTimer(16);
}

int BeingListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mBeings.size();
}

QVariant BeingListModel::data(const QModelIndex &index, int role) const
{
    const Being *being = mBeings.at(index.row());

    switch (role) {
    case BeingName:
        return being->name();
    case BeingChatMessage:
        return being->chatMessage();
    case BeingX:
        return being->x();
    case BeingY:
        return being->y();
    default:
        return QVariant();
    }
}

void BeingListModel::handleBeingEnter(MessageIn &message)
{
    const int type = message.readInt8();
    const int id = message.readInt16();
    message.readInt8(); // action
    const int x = message.readInt16();
    const int y = message.readInt16();

    Being *being = new Being(type, id, QPointF(x, y));

    if (being->type() == OBJECT_CHARACTER) {
        being->setName(message.readString());

        // Match the being by name to see whether it's the current player
        if (being->name() == mPlayerName) {
            mPlayerBeing = being;
            emit playerChanged();
        }
    }

    qDebug() << Q_FUNC_INFO << being->name() << being->id() << being->x() << being->y();
    addBeing(being);
}

void BeingListModel::handleBeingLeave(MessageIn &message)
{
    const int id = message.readInt16();

    if (mPlayerBeing->id() == id) {
        mPlayerBeing = 0;
        emit playerChanged();
    }

    removeBeing(id);
}

void BeingListModel::handleBeingsMove(MessageIn &message)
{
    while (message.unreadLength()) {
        const int id = message.readInt16();
        const int flags = message.readInt8();

        if (!(flags & MOVING_POSITION))
            continue;

        const int sx = message.readInt16();
        const int sy = message.readInt16();
        const int speed = message.readInt8();

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

        being->setServerPosition(QPointF(sx, sy));
    }
}

void BeingListModel::handleBeingActionChange(MessageIn &message)
{
    const int id = message.readInt16();
    if (Being *being = beingById(id)) {
        const Being::Action action = (Being::Action) message.readInt8();
        being->setAction(action);
    }
}

void BeingListModel::handleBeingSay(MessageIn &message)
{
    const int id = message.readInt16();
    const int index = indexOfBeing(id);

    if (index != -1) {
        const QString text = message.readString();
        mBeings.at(index)->say(text);
        notifyBeingChanged(index);
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
        if (pos == target)
            continue;

        const qreal walkSpeed = being->walkSpeed();
        QVector2D direction(target - pos);

        if (direction.lengthSquared() < walkSpeed * walkSpeed) {
            being->setPosition(target);
            notifyBeingChanged(i);
            continue;
        }

        direction.normalize();
        direction *= walkSpeed;
        being->setPosition(pos + direction.toPointF());
        notifyBeingChanged(i);
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

void BeingListModel::notifyBeingChanged(int index)
{
    const QModelIndex modelIndex = QAbstractListModel::index(index);
    emit dataChanged(modelIndex, modelIndex);
}
