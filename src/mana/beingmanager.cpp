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

#include "beingmanager.h"

#include "beinglistmodel.h"
#include "messagein.h"
#include "protocol.h"

#include <QTimerEvent>
#include <QVector2D>
#include <QDebug>

using namespace Mana;

Being::Being(int type, int id)
    : mType(type)
    , mId(id)
    , mWalkSpeed(0.0)
{
}

void Being::setPosition(QPointF position)
{
    if (mPosition == position)
        return;

    mPosition = position;
    emit positionChanged();
}

void Being::setServerPosition(QPointF position)
{
    mServerPosition = position;
}

void Being::setName(const QString &name)
{
    if (mName == name)
        return;

    mName = name;
    emit nameChanged();
}


BeingManager::BeingManager(QObject *parent)
    : QObject(parent)
    , mBeingListModel(new BeingListModel(this))
    , mPlayerBeing(0)
{
    mBeingUpdateTimer = startTimer(16);
}

void BeingManager::handleBeingEnter(MessageIn &message)
{
    const int type = message.readInt8();
    const int id = message.readInt16();
    message.readInt8(); // action
    const int x = message.readInt16();
    const int y = message.readInt16();

    Being *being = new Being(type, id);
    being->setPosition(QPointF(x, y));

    if (being->type() == OBJECT_PLAYER) {
        being->setName(message.readString());

        // Match the being by name to see whether it's the current player
        if (being->name() == mPlayerName) {
            mPlayerBeing = being;
            emit playerChanged();
        }
    }

    qDebug() << Q_FUNC_INFO << being->name() << being->id() << being->x() << being->y();
    mBeingListModel->addBeing(being);
}

void BeingManager::handleBeingLeave(MessageIn &message)
{
    const int id = message.readInt16();

    if (mPlayerBeing->id() == id) {
        mPlayerBeing = 0;
        emit playerChanged();
    }

    mBeingListModel->removeBeing(id);
}

void BeingManager::handleBeingsMove(MessageIn &message)
{
    while (message.unreadLength()) {
        const int id = message.readInt16();
        const int flags = message.readInt8();

        if (!(flags & MOVING_POSITION))
            continue;

        const int sx = message.readInt16();
        const int sy = message.readInt16();
        const int speed = message.readInt8();

        const int beingIndex = mBeingListModel->indexOfBeing(id);
        if (beingIndex == -1)
            continue;

        Being *being = mBeingListModel->beingAt(beingIndex);

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

void BeingManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != mBeingUpdateTimer)
        return;

    const QList<Being*> &beings = mBeingListModel->beings();
    for (int i = 0, end = beings.size(); i < end; ++i) {
        Being *being = beings.at(i);

        const QPointF pos = being->position();
        const QPointF target = being->serverPosition();
        if (pos == target)
            continue;

        const qreal walkSpeed = being->walkSpeed();
        QVector2D direction(target - pos);

        if (direction.lengthSquared() + 1 < walkSpeed * walkSpeed) {
            being->setPosition(target);
            mBeingListModel->notifyBeingChanged(i);
            continue;
        }

        direction.normalize();
        direction *= walkSpeed;
        being->setPosition(pos + direction.toPointF());
        mBeingListModel->notifyBeingChanged(i);
    }
}
