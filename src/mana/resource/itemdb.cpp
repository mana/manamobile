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

#include "mana/resourcemanager.h"
#include "mana/xmlreader.h"

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

static ItemInfo::Type itemTypeFromString(const QStringRef &name)
{
    if      (name == "generic")         return ItemInfo::ITEM_UNUSABLE;
    else if (name == "usable")          return ItemInfo::ITEM_USABLE;
    else if (name == "equip-1hand")     return ItemInfo::ITEM_EQUIPMENT_ONE_HAND_WEAPON;
    else if (name == "equip-2hand")     return ItemInfo::ITEM_EQUIPMENT_TWO_HANDS_WEAPON;
    else if (name == "equip-torso")     return ItemInfo::ITEM_EQUIPMENT_TORSO;
    else if (name == "equip-arms")      return ItemInfo::ITEM_EQUIPMENT_ARMS;
    else if (name == "equip-head")      return ItemInfo::ITEM_EQUIPMENT_HEAD;
    else if (name == "equip-legs")      return ItemInfo::ITEM_EQUIPMENT_LEGS;
    else if (name == "equip-shield")    return ItemInfo::ITEM_EQUIPMENT_SHIELD;
    else if (name == "equip-ring")      return ItemInfo::ITEM_EQUIPMENT_RING;
    else if (name == "equip-charm")     return ItemInfo::ITEM_EQUIPMENT_CHARM;
    else if (name == "equip-necklace")  return ItemInfo::ITEM_EQUIPMENT_NECKLACE;
    else if (name == "equip-feet")      return ItemInfo::ITEM_EQUIPMENT_FEET;
    else if (name == "equip-ammo")      return ItemInfo::ITEM_EQUIPMENT_AMMO;
    else if (name == "racesprite")      return ItemInfo::ITEM_SPRITE_RACE;
    else if (name == "hairsprite")      return ItemInfo::ITEM_SPRITE_HAIR;

    return ItemInfo::ITEM_UNUSABLE;
}

static BeingGender genderFromString(const QStringRef &gender)
{
    if      (gender == "male")          return GENDER_MALE;
    else if (gender == "female")        return GENDER_FEMALE;

    return GENDER_UNSPECIFIED;
}

ItemDB *ItemDB::mInstance = 0;

ItemDB::ItemDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
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
    emit itemsChanged();
}

const ItemInfo *ItemDB::getInfo(int id) const
{
    return mItems.value(id);
}

static ItemInfo *readItem(XmlReader &xml)
{
    const QXmlStreamAttributes attr = xml.attributes();
    int id = attr.value("id").toString().toInt();

    if (!id) {
        qDebug() << "Bad or missing item id at line " << xml.lineNumber();
        xml.skipCurrentElement();
        return 0;
    }

    // TODO: Move races to a seperate file and move parsing to racedb
    if (attr.value("type") == "racesprite") { // Race "item"
        RaceInfo *raceInfo = new RaceInfo(-id);
        raceInfo->setName(attr.value("name").toString());

        while (xml.readNextStartElement()) {
            if (xml.name() == "sprite") {
                const BeingGender gender =
                        genderFromString(xml.attributes().value("gender"));
                SpriteReference *sprite =
                        SpriteReference::readSprite(xml, raceInfo);

                raceInfo->setSprite(gender, sprite);
            } else {
                xml.skipCurrentElement();
            }
        }

        RaceDB::instance()->setInfo(-id, raceInfo);
        return 0;
    }

    // TODO: Move hairs to a seperate file and move parsing to hairdb
    if (attr.value("type") == "hairsprite") { // Hair "item"
        // invert the negative id for now
        id = -id;
        HairInfo *hairInfo = new HairInfo(id, HairDB::instance());
        hairInfo->setName(attr.value("name").toString());

        while (xml.readNextStartElement()) {
            if (xml.name() == "sprite") {
                const BeingGender gender =
                        genderFromString(xml.attributes().value("gender"));
                SpriteReference *sprite =
                        SpriteReference::readSprite(xml, hairInfo);

                hairInfo->setSprite(gender, sprite);
            } else {
                xml.skipCurrentElement();
            }
        }

        HairDB::instance()->setInfo(id, hairInfo);
        return 0;
    }

    ItemInfo *item = new ItemInfo(id);

    item->setType(itemTypeFromString(attr.value("type")));
    item->setName(attr.value("name").toString());
    item->setDescription(attr.value("description").toString());
    item->setWeight(attr.value("weight").toString().toInt());

    SpriteDisplay display;
    display.image = attr.value("image").toString();

    if (item->name().isEmpty())
        item->setName("unnamed");

    QStringList effects;

    for (int i = 0; i < int(sizeof(fields) / sizeof(fields[0])); ++i) {
        int value = attr.value(fields[i].tag).toString().toInt();
        if (value)
            effects.append(fields[i].format.arg(value));
    }

//    foreach (Stat stat, mExtraStats) {
//        int value = xml.intAttribute(stat.tag);
//        if (value)
//            effects.append(stat.format.arg(value));
//    }

    const QStringRef temp = attr.value("effect");
    if (!temp.isEmpty())
        effects.append(temp.toString());

    item->setEffects(effects);

    while (xml.readNextStartElement()) {
        if (xml.name() == "sprite") {
            const BeingGender gender =
                    genderFromString(xml.attributes().value("gender"));
            SpriteReference *sprite = SpriteReference::readSprite(xml, item);

            item->addSprite(gender, sprite);
        } else if (xml.name() == "particlefx") {
            item->setParticleFx(xml.readElementText());
        } else if (xml.name() == "sound") {
            xml.skipCurrentElement(); // TODO
        } else if (xml.name() == "floor") {
            while (xml.readNextStartElement()) {
                if (xml.name() == "sprite") {
                    display.sprites.append(
                                SpriteReference::readSprite(xml, item));
                } else if (xml.name() == "particlefx") {
                    display.particles.append(xml.readElementText());
                } else {
                    xml.readUnknownElement();
                }
            }
        } else {
            xml.skipCurrentElement();
        }
    }

    item->setDisplay(display);

    return item;
}

void ItemDB::fileReady()
{
    XmlReader xml(mReply);

    if (!xml.readNextStartElement() || xml.name() != "items") {
        qWarning() << "Error loading items.xml";
        return;
    }

    while (xml.readNextStartElement()) {
        if (xml.name() == "item") {
            if (ItemInfo *item = readItem(xml))
                mItems[item->id()] = item;
        } else if (xml.name() == "version") {
            xml.skipCurrentElement(); // Ignore for now
        } else {
            xml.readUnknownElement();
        }
    }

    mLoaded = true;
    RaceDB::instance()->mLoaded = true;
    HairDB::instance()->mLoaded = true;
    emit itemsChanged();
    emit loaded();
    emit RaceDB::instance()->racesChanged();
    emit RaceDB::instance()->loaded();
    emit HairDB::instance()->hairsChanged();
    emit HairDB::instance()->loaded();
}
