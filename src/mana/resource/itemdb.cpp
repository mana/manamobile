/*
 * manalib
 * Copyright 2011, Jared Adams <jaxad0127@gmail.com>
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

#include "itemdb.h"
#include "../../resourcemanager.h"
#include "../xmlreader.h"

#include "hairdb.h"
#include "racedb.h"

#include <QDebug>
#include <QNetworkReply>

using namespace Mana;

static const ItemDB::Stat fields[] = {
    ItemDB::Stat("attack",  "Attack %+d"),
    ItemDB::Stat("defense", "Defense %+d"),
    ItemDB::Stat("hp",      "HP %+d"),
    ItemDB::Stat("mp",      "MP %+d")
};

void ItemDB::setStatsList(QList<Stat> stats)
{
    mExtraStats = stats;
}

QList<const ItemInfo*> ItemDB::items() const
{
    return mItems.values();
}

static ItemInfo::Type itemTypeFromString(QString name)
{
    if      (name=="generic")           return ItemInfo::ITEM_UNUSABLE;
    else if (name=="usable")            return ItemInfo::ITEM_USABLE;
    else if (name=="equip-1hand")       return ItemInfo::ITEM_EQUIPMENT_ONE_HAND_WEAPON;
    else if (name=="equip-2hand")       return ItemInfo::ITEM_EQUIPMENT_TWO_HANDS_WEAPON;
    else if (name=="equip-torso")       return ItemInfo::ITEM_EQUIPMENT_TORSO;
    else if (name=="equip-arms")        return ItemInfo::ITEM_EQUIPMENT_ARMS;
    else if (name=="equip-head")        return ItemInfo::ITEM_EQUIPMENT_HEAD;
    else if (name=="equip-legs")        return ItemInfo::ITEM_EQUIPMENT_LEGS;
    else if (name=="equip-shield")      return ItemInfo::ITEM_EQUIPMENT_SHIELD;
    else if (name=="equip-ring")        return ItemInfo::ITEM_EQUIPMENT_RING;
    else if (name=="equip-charm")       return ItemInfo::ITEM_EQUIPMENT_CHARM;
    else if (name=="equip-necklace")    return ItemInfo::ITEM_EQUIPMENT_NECKLACE;
    else if (name=="equip-feet")        return ItemInfo::ITEM_EQUIPMENT_FEET;
    else if (name=="equip-ammo")        return ItemInfo::ITEM_EQUIPMENT_AMMO;
    else if (name=="racesprite")        return ItemInfo::ITEM_SPRITE_RACE;
    else if (name=="hairsprite")        return ItemInfo::ITEM_SPRITE_HAIR;
    else return ItemInfo::ITEM_UNUSABLE;
}

ItemInfo *ItemInfo::null = 0;

ItemDB *ItemDB::mInstance = 0;

ItemDB::ItemDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
    ItemInfo::null = new ItemInfo(0);

    Q_ASSERT(!mInstance);
    mInstance = this;
}

void ItemDB::load()
{
    if (mLoaded)
        return;

    mReply = ResourceManager::instance()->requestFile(ITEMS_DB_FILE);

    connect(mReply, SIGNAL(finished()), this, SLOT(fileReady()));
}

void ItemDB::unload()
{
    qDeleteAll(mItems);
    mItems.clear();

    mLoaded = false;
}

const ItemInfo *ItemDB::getInfo(int id) const
{
    if (mItems.contains(id))
        return mItems[id];

    return ItemInfo::null;
}

void ItemDB::fileReady()
{
    XmlReader xml(mReply);

    if (xml.readNextStartElement() && xml.name() == "items")
    {}
    else
    {
        xml.raiseError(tr("Not an item database."));
        return;
    }

    QStringRef currentTag;
    ItemInfo *currentItem;

    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isStartElement())
            currentTag = xml.name();
        else
            continue;

        if (currentTag == "item")
        {
            int id = xml.intAttribute("id", 0);

            if (!id)
            {
                qDebug() << "Bad item id at line " << xml.lineNumber();
                xml.skipCurrentElement();
                continue;
            }

            // TODO: Move races to a seperate file and move parsing to racedb
            if (xml.attribute("type") == "racesprite") { // Race "item"
                RaceInfo *raceInfo = new RaceInfo(-id);
                raceInfo->mName = xml.attribute("name");
                while (!(xml.name() == "item" && xml.isEndElement())) {
                    xml.readNext();
                    if (!xml.isStartElement())
                        continue;

                    if (xml.name() == "sprite") {
                        QString gender = xml.attribute("gender");
                        SpriteReference *sprite =
                                SpriteReference::readSprite(xml, raceInfo);
                        if (gender == "male")
                            raceInfo->mSprites[GENDER_MALE] = sprite;
                        else if (gender == "female")
                            raceInfo->mSprites[GENDER_FEMALE] = sprite;
                        else
                            raceInfo->mSprites[GENDER_UNSPECIFIED] = sprite;
                    }
                }

                RaceDB::instance()->mRaces[-id] = raceInfo;
                continue;
            }

            // TODO: Move hairs to a seperate file and move parsing to hairdb
            if (xml.attribute("type") == "hairsprite") { // Hair "item"
                HairInfo *hairInfo = new HairInfo(-id, HairDB::instance());
                hairInfo->mName = xml.attribute("name");
                while (!(xml.name() == "item" && xml.isEndElement())) {
                    xml.readNext();
                    if (!xml.isStartElement())
                        continue;

                    if (xml.name() == "sprite") {
                        QString gender = xml.attribute("gender");
                        SpriteReference *sprite =
                                SpriteReference::readSprite(xml, hairInfo);
                        if (gender == "male")
                            hairInfo->mSprites[GENDER_MALE] = sprite;
                        else if (gender == "female")
                            hairInfo->mSprites[GENDER_FEMALE] = sprite;
                        else
                            hairInfo->mSprites[GENDER_UNSPECIFIED] = sprite;
                    }
                }

                HairDB::instance()->mHairs[-id] = hairInfo;
                continue;
            }

            currentItem = new ItemInfo(id);

            currentItem->mType = itemTypeFromString(xml.attribute("type"));
            currentItem->mName = xml.attribute("name", "unnamed");
            currentItem->mDescription = xml.attribute("description");
            currentItem->mWeight = xml.intAttribute("weight");

            currentItem->mDisplay.image = xml.attribute("image");

            for (int i = 0; i < int(sizeof(fields) / sizeof(fields[0])); ++i)
            {
                int value = xml.intAttribute(fields[i].tag);
                if (value)
                    currentItem->mEffects.push_back(fields[i].format.arg(value));
            }

            foreach(Stat stat, mExtraStats)
            {
                int value = xml.intAttribute(stat.tag);
                if (value)
                    currentItem->mEffects.push_back(stat.format.arg(value));
            }

            QString temp = xml.attribute("effect");
            if (!temp.isEmpty())
                currentItem->mEffects.push_back(temp);

            // TODO read other attributes?

            mItems[id] = currentItem;
        }
        else if (currentTag == "sprite")
        {
            QString gender = xml.attributes().value("gender").toString();
            SpriteReference *sprite = SpriteReference::readSprite(xml, currentItem);

            if (gender == "male")
                currentItem->mSprites[GENDER_MALE] = sprite;
            else if (gender == "female")
                currentItem->mSprites[GENDER_FEMALE] = sprite;
            else
                currentItem->mSprites[GENDER_UNSPECIFIED] = sprite;
        }
        else if (currentTag == "particlefx")
            currentItem->mParticleFx = xml.readElementText();
        else if (currentTag == "sound")
            xml.skipCurrentElement(); // TODO
        else if (currentTag == "floor")
            while (xml.readNextStartElement())
            {
                if (xml.name() == "sprite")
                    currentItem->mDisplay.sprites.push_back(
                                SpriteReference::readSprite(xml, currentItem));
                else if (xml.name() == "particlefx")
                    currentItem->mDisplay.particles.push_back(xml.readElementText());
                else
                    xml.readUnknownElement();
            }
        else if (currentTag == "version")
            xml.skipCurrentElement(); // Ignore for now
        else
            xml.readUnknownElement();
    }

    mLoaded = true;
    RaceDB::instance()->mLoaded = true;
    HairDB::instance()->mLoaded = true;
    emit itemsChanged();
    emit RaceDB::instance()->racesChanged();
    emit HairDB::instance()->hairsChanged();
}
