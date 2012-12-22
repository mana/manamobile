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

#include "beingitem.h"

#include "resourcemanager.h"

#include <QPainter>

#include "spriteitem.h"

#include "mana/being.h"
#include "mana/gameclient.h"

#include "mana/resource/hairdb.h"
#include "mana/resource/itemdb.h"
#include "mana/resource/racedb.h"
#include "mana/resource/spritedef.h"

using namespace Mana;

BeingItem::BeingItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
    , mBeing(0)
    , mHairSprite(0)
    , mRaceSprite(0)
{
}

BeingItem::~BeingItem()
{
}

void BeingItem::loadHairSprite()
{
    HairDB *db = HairDB::instance();
    if (!db->loaded() || mHairSprite || !mBeing)
        return;

    const HairInfo *hair = db->getInfo(mBeing->hairStyle());

    qDebug() << "GETTING HAIR!!!!!" << hair
             << " Desired: " << mBeing->hairStyle();

    if (!hair)
        return;

    mHairSprite = new SpriteItem(hair->sprite(mBeing->gender()), this);
    mHairSprite->setZValue(0);

    mHairSprite->setDirection(spriteDirectionByBeing(mBeing->direction()));
    mHairSprite->play(mBeing->action());

    disconnect(db, SIGNAL(hairsChanged()), this, SLOT(loadHairSprite()));
}

void BeingItem::loadRaceSprite()
{
    RaceDB *db = RaceDB::instance();
    if (!db->loaded() || mRaceSprite)
        return;

    // No real race support so far. Just load the first race
    Q_ASSERT(db->races().count() > 0);
    mRaceSprite = new SpriteItem(db->races().at(0)->sprite(mBeing->gender()),
                                 this);
    mRaceSprite->setZValue(-1);

    mRaceSprite->setDirection(spriteDirectionByBeing(mBeing->direction()));
    mRaceSprite->play(mBeing->action());

    disconnect(db, SIGNAL(racesChanged()), this, SLOT(loadRaceSprite()));
}

void BeingItem::setBeing(Being *being)
{
    if (mBeing == being)
        return;

    if (mBeing) {
        disconnect(mBeing, SIGNAL(slotEquipped(int,int)),
                   this, SLOT(slotEquipped(int,int)));
        disconnect(mBeing, SIGNAL(slotUnequipping(int)),
                   this, SLOT(slotUnequipping(int)));
        disconnect(mBeing, SIGNAL(actionChanged(QString)),
                   this, SLOT(actionChanged(QString)));
        disconnect(mBeing, SIGNAL(directionChanged(Mana::BeingDirection)),
                   this, SLOT(directionChanged(Mana::BeingDirection)));
        disconnect(RaceDB::instance(), SIGNAL(racesChanged()),
                   this, SLOT(loadRace()));

        QMap<int, int> &equipmentSlots = mBeing->equipmentSlots();
        for (QMap<int, int>::iterator it = equipmentSlots.begin(),
             it_end = equipmentSlots.end(); it != it_end; ++it)
        {
            slotUnequipping(it.key());
        }
    }
    mBeing = being;
    if (mBeing) {
        connect(mBeing, SIGNAL(slotEquipped(int,int)),
                this, SLOT(slotEquipped(int,int)));
        connect(mBeing, SIGNAL(slotUnequipping(int)),
                this, SLOT(slotUnequipping(int)));
        connect(mBeing, SIGNAL(actionChanged(QString)),
                this, SLOT(actionChanged(QString)));
        connect(mBeing, SIGNAL(directionChanged(Mana::BeingDirection)),
                this, SLOT(directionChanged(Mana::BeingDirection)));

        QMap<int, int> &equipmentSlots = mBeing->equipmentSlots();
        for (QMap<int, int>::iterator it = equipmentSlots.begin(),
             it_end = equipmentSlots.end(); it != it_end; ++it)
        {
            if (it.value()) // 0 means empty
                slotEquipped(it.key(), it.value());
        }

        // Update Racesprite
        RaceDB *raceDB = RaceDB::instance();
        if (raceDB->loaded())
            loadRaceSprite();
        else
            connect(raceDB, SIGNAL(racesChanged()), this, SLOT(loadRaceSprite()));


        // Update Hairsprite
        HairDB *hairDB = HairDB::instance();
        if (hairDB->loaded())
            loadHairSprite();
        else
            connect(hairDB, SIGNAL(hairsChanged()), this, SLOT(loadHairSprite()));
    }
}

void BeingItem::slotEquipped(int slot, int itemId)
{
    const ItemInfo *info = ItemDB::instance()->getInfo(itemId);
    if (info == ItemInfo::null)
        return;

    SpriteReference *spriteRef = info->sprite(mBeing->gender());
    if (!spriteRef)
        return;

    SpriteItem *spriteItem = new SpriteItem(spriteRef, this);
    spriteItem->setZValue(slot);
    mSprites[slot] = spriteItem;
    spriteItem->play(mBeing->action());
}

void BeingItem::slotUnequipping(int slot)
{
    QMap<int, SpriteItem *>::iterator it = mSprites.find(slot);
    if (it != mSprites.end()) {
        delete it.value();
        it = mSprites.erase(it);
    }
}

void BeingItem::actionChanged(const QString &newAction)
{
    foreach (SpriteItem *sprite, mSprites) {
        sprite->play(newAction);
    }
    if (mRaceSprite)
        mRaceSprite->play(newAction);
}

void BeingItem::directionChanged(BeingDirection newDirection)
{
    SpriteDirection spriteDirection = spriteDirectionByBeing(newDirection);

    foreach (SpriteItem *sprite, mSprites) {
        sprite->setDirection(spriteDirection);
    }

    if (mRaceSprite)
        mRaceSprite->setDirection(spriteDirection);
}
