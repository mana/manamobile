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

#include "character.h"

#include "protocol.h"
#include "spritelistmodel.h"

#include "resource/itemdb.h"
#include "resource/hairdb.h"
#include "resource/racedb.h"

#include <QDebug>

using namespace Mana;

Character::Character(int id, QPointF position)
    : Being(OBJECT_CHARACTER, id, position)
    , mHairStyle(0)
    , mHairColor(0)
{
}

void Character::setEquipmentSlot(int slot, int itemId)
{
    if (mEquipmentSlots[slot] == itemId)
        return;

    QMap<int, int>::iterator it = mEquipmentSlots.find(slot);
    if (it != mEquipmentSlots.end()) {
        emit slotUnequipping(slot);
        it = mEquipmentSlots.erase(it);
        mSpriteList->removeSprite(SLOT_EQUIPMENT + slot);
    }

    if (itemId) {
        mEquipmentSlots[slot] = itemId;
        emit slotEquipped(slot, itemId);

        Q_ASSERT(ItemDB::instance()->loaded());
        const ItemInfo *info = ItemDB::instance()->getInfo(itemId);
        if (!info) {
            qWarning() << Q_FUNC_INFO << "Tried to equip unknown itemid "
                       << itemId;
            return;
        }
        SpriteReference *sprite = info->sprite(mGender);
        if (sprite)
            mSpriteList->addSprite(SLOT_EQUIPMENT + slot, sprite);
    }
}

void Character::setHairStyle(int style, int color)
{
    if (mHairStyle != style || mHairColor != color) {
        mHairStyle = style;
        mHairColor = color;
        emit hairChanged();
        Q_ASSERT(HairDB::instance()->loaded());
        const HairInfo *info = HairDB::instance()->getInfo(style);
        SpriteReference *sprite = info->sprite(mGender);
        if (sprite)
            mSpriteList->addSprite(SLOT_HAIR, sprite);
    }
}

void Character::setGender(BeingGender gender)
{
    Being::setGender(gender);
    rebuildSprites();
}

void Character::rebuildSprites()
{
    mSpriteList->removeAll();

    SpriteReference *sprite;

    // Hair
    const HairInfo *hairInfo = HairDB::instance()->getInfo(mHairStyle);
    sprite = hairInfo->sprite(mGender);
    if (sprite)
        mSpriteList->addSprite(SLOT_HAIR, sprite);

    // Race
    RaceInfo *raceInfo = RaceDB::instance()->races().at(0);
    sprite = raceInfo->sprite(mGender);
    if (sprite)
        mSpriteList->addSprite(SLOT_RACE, sprite);
}

