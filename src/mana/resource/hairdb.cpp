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

#include "hairdb.h"

using namespace Mana;

HairDB *HairDB::mInstance = 0;

HairDB::HairDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

void HairDB::load()
{
    // Will later load the race xml. Currently this is handeled via items.xml
    // TODO: Implement races with own xml file
}

void HairDB::unload()
{
    qDeleteAll(mHairs);
    mHairs.clear();

    mLoaded = false;
    emit hairsChanged();
}
