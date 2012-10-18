/*
 * manalib
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

#include "action.h"

#include "animation.h"

using namespace Mana;

Action::Action(QObject *parent) :
    QObject(parent)
{
}

Action::~Action()
{
    qDeleteAll(mAnimations);
}

void Action::setAnimation(int direction, Animation *animation)
{
    mAnimations[direction] = animation;
}

const Animation *Action::animation(int direction) const
{
    QMap<int, Animation *>::const_iterator it = mAnimations.find(direction);

    // When the given direction is not available, return the first one.
    // (either DEFAULT, or more usually DOWN).
    if (it == mAnimations.end())
        it = mAnimations.begin();

    return (it == mAnimations.end()) ? 0 : it.value();
}

