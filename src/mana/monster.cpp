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

#include "monster.h"

#include "spritelistmodel.h"

#include "resource/monsterdb.h"

using namespace Mana;

Monster::Monster(int monsterId)
    : Being(OBJECT_MONSTER)
    , mMonsterId(monsterId)
{
    if (MonsterDB::instance()->isLoaded()) {
        initializeSprites();
    } else {
        connect(MonsterDB::instance(), SIGNAL(loaded()),
                this, SLOT(initializeSprites()));
    }
}

void Monster::initializeSprites()
{
    Q_ASSERT(MonsterDB::instance()->isLoaded());

    if (const MonsterInfo *monsterInfo = MonsterDB::instance()->getInfo(mMonsterId))
        mSpriteList->setSprites(monsterInfo->sprites());
}
