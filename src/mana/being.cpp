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

#include "spritelistmodel.h"

#include "resource/spritedef.h"

#include <cmath>

using namespace Mana;

Being::Being(int type)
    : mType(type)
    , mId(0)
    , mWalkSpeed(0.0)
    , mAction(SpriteAction::STAND)
    , mDirection(DOWN)
    , mGender(GENDER_UNSPECIFIED)
{
    mSpriteList = new SpriteListModel(this);
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

Action::SpriteDirection Being::spriteDirection() const
{
    return beingToSpriteDirection(mDirection);
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
        emit actionChanged();
    }
}

void Being::lookAt(const QPointF &point)
{
    if (mPosition == point)
        return;

    const QPointF diff = point - mPosition;

    if (std::abs(diff.x()) > std::abs(diff.y()))
        setDirection((diff.x() < 0) ? LEFT : RIGHT);
    else
        setDirection((diff.y() < 0) ? UP : DOWN);
}
