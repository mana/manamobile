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

#include "being.h"

#include "resource/spritedef.h"

using namespace Mana;

Being::Being(int type, int id, QPointF position)
    : mType(type)
    , mId(id)
    , mWalkSpeed(0.0)
    , mAction(SpriteAction::STAND)
    , mPosition(position)
    , mServerPosition(position)
    , mGender(GENDER_UNSPECIFIED)
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

void Being::setDirection(BeingDirection direction)
{
    if (mDirection == direction)
        return;

    mDirection = direction;
    emit directionChanged(direction);
}

void Being::setName(const QString &name)
{
    if (mName == name)
        return;

    mName = name;
    emit nameChanged();
}

void Being::say(const QString &text)
{
    mChatMessage = text;
    emit chatMessageChanged();
}

void Being::setAction(const QString &action)
{
    if (mAction != action) {
        mAction = action;
        emit actionChanged(action);
    }
}

void Being::setSprite(int slot, int itemId)
{
    if (mSlots[slot] == itemId)
        return;

    QMap<int, int>::iterator it = mSlots.find(slot);
    if (it != mSlots.end()) {
        emit slotUnequipping(slot);
        it = mSlots.erase(it);
    }

    if (itemId) {
        mSlots[slot] = itemId;
        emit slotEquipped(slot, itemId);
    }
}

void Being::lookAt(const QPointF &point)
{
    // We first handle simple cases

    // If the two positions are the same,
    // don't update the direction since it's only a matter of keeping
    // the previous one.
    if (mPosition.x() == point.x() && mPosition.y() == point.y())
        return;

    if (mPosition.x() == point.x()) {
        if (mPosition.y() > point.y())
            setDirection(UP);
        else
            setDirection(DOWN);
        return;
    }

    if (mPosition.y() == point.y()) {
        if (mPosition.x() > point.x())
            setDirection(LEFT);
        else
            setDirection(RIGHT);
        return;
    }

    // Now let's handle diagonal cases
    // First, find the lower angle:
    if (mPosition.x() < point.x()) {
        // Up-right direction
        if (mPosition.y() > point.y()) {
            // Compute tan of the angle
            if ((mPosition.y() - point.y()) / (point.x() - mPosition.x()) < 1)
                // The angle is less than 45°, we look to the right
                setDirection(RIGHT);
            else
                setDirection(UP);
            return;
        }
        else { // Down-right
            // Compute tan of the angle
            if ((point.y() - mPosition.y()) / (point.x() - mPosition.x()) < 1)
                // The angle is less than 45°, we look to the right
                setDirection(RIGHT);
            else
                setDirection(DOWN);
            return;
        }
    }
    else
    {
        // Up-left direction
        if (mPosition.y() > point.y()) {
            // Compute tan of the angle
            if ((mPosition.y() - point.y()) / (mPosition.x() - point.x()) < 1)
                // The angle is less than 45°, we look to the left
                setDirection(LEFT);
            else
                setDirection(UP);
            return;
        }
        else { // Down-left
            // Compute tan of the angle
            if ((point.y() - mPosition.y()) / (mPosition.x() - point.x()) < 1)
                // The angle is less than 45°, we look to the left
                setDirection(LEFT);
            else
                setDirection(DOWN);
            return;
        }
    }
}
