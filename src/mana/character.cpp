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

Character::Character()
    : Being(OBJECT_CHARACTER)
    , mHairStyle(0)
    , mHairColor(0)
    , mCharacterSlot(0)
    , mLevel(0)
{
    if (!HairDB::instance()->isLoaded())
        connect(HairDB::instance(), SIGNAL(loaded()), SLOT(rebuildSprites()));
    if (!RaceDB::instance()->isLoaded())
        connect(RaceDB::instance(), SIGNAL(loaded()), SLOT(rebuildSprites()));
    if (!ItemDB::instance()->isLoaded())
        connect(ItemDB::instance(), SIGNAL(loaded()), SLOT(rebuildSprites()));
}

void Character::setEquipmentSlots(const QMap<int, int> &equipmentSlots)
{
    QMap<int, int> toInsert(equipmentSlots);

    // Delete old entries and update existing ones if needed
    for (QMap<int, int>::iterator it = mEquipmentSlots.begin(),
         it_end = mEquipmentSlots.end(); it != it_end;) {
        QMap<int, int>::iterator slotIt = toInsert.find(it.key());
        qDebug() << it.key();
        if (slotIt == toInsert.end()) {
            qDebug() << "a" << it.key();
            mSpriteList->removeSprite(SLOT_EQUIPMENT + it.key());
            Q_ASSERT(it != mEquipmentSlots.end());
            it = mEquipmentSlots.erase(it);
            continue;
        }
        if (it.key() != slotIt.key()) {
            mSpriteList->removeSprite(SLOT_EQUIPMENT + it.key());
            mEquipmentSlots[it.key()] = slotIt.value();

            slotIt = toInsert.erase(slotIt);

            if (ItemDB::instance()->isLoaded()) {
                ItemDB *itemDB = ItemDB::instance();
                const ItemInfo *info = itemDB->getInfo(slotIt.value());
                if (!info) {
                    qWarning() << Q_FUNC_INFO << "Tried to equip unknown item "
                               << slotIt.value();
                    return;
                }
                foreach (SpriteReference *sprite, info->sprites(mGender))
                    mSpriteList->addSprite(SLOT_EQUIPMENT + it.key(), sprite);
            }
        }
        ++it;
    }

    // Insert remaining ones
    for (QMap<int, int>::iterator it = toInsert.begin(),
         it_end = toInsert.end(); it != it_end; ++it) {
        mEquipmentSlots[it.key()] = it.value();

        if (ItemDB::instance()->isLoaded()) {
            const ItemInfo *info = ItemDB::instance()->getInfo(it.value());
            if (!info) {
                qWarning() << Q_FUNC_INFO << "Tried to equip unknown item "
                           << it.value();
                return;
            }
            QVector<SpriteReference *> sprites = info->sprites(mGender);
            foreach (SpriteReference *sprite, sprites)
                mSpriteList->addSprite(SLOT_EQUIPMENT + it.key(), sprite);
        }
    }
}

void Character::setHairStyle(int style, int color)
{
    if (mHairStyle == style && mHairColor == color)
        return;

    mHairStyle = style;
    mHairColor = color;
    emit hairChanged();

    if (HairDB::instance()->isLoaded()) {
        if (const HairInfo *info = HairDB::instance()->getInfo(style))
            if (const SpriteReference *sprite = info->sprite(mGender))
                mSpriteList->setSprite(SLOT_HAIR, sprite);
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

    if (HairDB::instance()->isLoaded()) {
        if (const HairInfo *hairInfo = HairDB::instance()->getInfo(mHairStyle))
            if (const SpriteReference *sprite = hairInfo->sprite(mGender))
                mSpriteList->addSprite(SLOT_HAIR, sprite);
    }

    if (RaceDB::instance()->isLoaded()) {
        const RaceInfo *raceInfo = RaceDB::instance()->races().at(0);
        if (const SpriteReference *sprite = raceInfo->sprite(mGender))
            mSpriteList->addSprite(SLOT_RACE, sprite);
    }

    if (ItemDB::instance()->isLoaded()) {
        QMapIterator<int, int> it(mEquipmentSlots);
        while (it.hasNext()) {
            it.next();
            const int slot = it.key();
            const int itemId = it.value();

            const ItemInfo *info = ItemDB::instance()->getInfo(itemId);
            if (!info) {
                qWarning() << Q_FUNC_INFO << "Unknown equipped item "
                           << itemId;
                continue;
            }
            QVector<SpriteReference *> sprites = info->sprites(mGender);
            foreach (SpriteReference *sprite, sprites)
                mSpriteList->addSprite(SLOT_EQUIPMENT + slot, sprite);
        }
    }
}
