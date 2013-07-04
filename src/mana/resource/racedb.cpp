/*
 * Mana QML plugin
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

#include "racedb.h"

using namespace Mana;

RaceDB *RaceDB::mInstance = 0;

RaceDB::RaceDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

void RaceDB::load()
{
    // Will later load the race xml. Currently this is handeled via items.xml
    // TODO: Implement races with own xml file
}

void RaceDB::unload()
{
    qDeleteAll(mRaces);
    mRaces.clear();

    mLoaded = false;
    emit racesChanged();
}
