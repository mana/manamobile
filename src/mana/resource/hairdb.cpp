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
    qDeleteAll(mHairMap);
    mHairMap.clear();
    mHairList.clear();

    mLoaded = false;
    emit hairsChanged();
}

void HairDB::setInfo(int id, HairInfo *info)
{
    mHairMap[id] = info;
    mHairList.append(info);
}

QQmlListProperty<HairInfo> HairDB::hairs()
{
    return QQmlListProperty<HairInfo>(this, 0,
                                      HairDB::hairs_count,
                                      HairDB::hairs_at);
}

int HairDB::hairs_count(QQmlListProperty<HairInfo> *)
{
    return mInstance->mHairList.size();
}

HairInfo *HairDB::hairs_at(QQmlListProperty<HairInfo> *, int index)
{
    return mInstance->mHairList.at(index);
}
