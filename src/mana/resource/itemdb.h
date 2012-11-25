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

#ifndef ITEMDB_H
#define ITEMDB_H

#include "spritedef.h"

#include "mana/protocol.h"

#include <QList>
#include <QMap>

class XmlReader;

#define ITEMS_DB_FILE "items.xml"

class QNetworkReply;

namespace Mana {

class ItemInfo;

class ItemDB : public QObject
{
Q_OBJECT

Q_PROPERTY(QList<const ItemInfo*> items READ items NOTIFY itemsChanged)

friend class ItemInfo;
friend class RaceInfo;

public:
    class Stat
    {
        friend class ItemDB;

    public:
        Stat(QString tag, QString format) : tag(tag), format(format) {}

        bool operator ==(QString name) const { return tag == name; }

    protected:
        QString tag;
        QString format;
    };

    ItemDB(QObject *parent);

    Q_INVOKABLE void load();

    Q_INVOKABLE void unload();

    Q_INVOKABLE const ItemInfo *getInfo(int id) const;

    bool loaded() const { return mLoaded; }

    void setStatsList(QList<Stat> stats);

    QList<const ItemInfo*> items() const;

    static ItemDB *instance() { return mInstance; }

signals:
    void itemsChanged();

private slots:
    void fileReady();

private:
    static ItemDB *mInstance;
    static SpriteReference *readSprite(XmlReader &xml, QObject *parent);

    ~ItemDB() { unload(); }

    QNetworkReply *mReply;
    bool mLoaded;
    QList<Stat> mExtraStats;
    QMap<int, const ItemInfo *> mItems;
};

class ItemInfo : public QObject
{
    Q_OBJECT
    friend class ItemDB;

    Q_PROPERTY(bool isNull READ isNull CONSTANT)

    Q_PROPERTY(int id READ id CONSTANT)

    Q_PROPERTY(ItemInfo::Type type READ type CONSTANT)

    Q_PROPERTY(SpriteDisplay display READ display CONSTANT)

    Q_PROPERTY(QString name READ name CONSTANT)

    Q_PROPERTY(QString description READ description CONSTANT)

    Q_PROPERTY(QList<QString> effects READ effects CONSTANT)

    Q_PROPERTY(int weight READ weight CONSTANT)

    Q_PROPERTY(QString particleFx READ particleFx CONSTANT)

public:
    /**
     * Enumeration of available Item types.
     */
    enum Type
    {
        ITEM_UNUSABLE = 0,
        ITEM_USABLE,
        ITEM_EQUIPMENT_ONE_HAND_WEAPON,
        ITEM_EQUIPMENT_TWO_HANDS_WEAPON,
        ITEM_EQUIPMENT_TORSO,
        ITEM_EQUIPMENT_ARMS, // 5
        ITEM_EQUIPMENT_HEAD,
        ITEM_EQUIPMENT_LEGS,
        ITEM_EQUIPMENT_SHIELD,
        ITEM_EQUIPMENT_RING,
        ITEM_EQUIPMENT_NECKLACE, // 10
        ITEM_EQUIPMENT_FEET,
        ITEM_EQUIPMENT_AMMO,
        ITEM_EQUIPMENT_CHARM,
        ITEM_SPRITE_RACE,
        ITEM_SPRITE_HAIR // 15
    };

    Q_ENUMS(Type)

    static ItemInfo *null;

    bool isNull() const
    { return this == null; }

    int id() const
    { return mId; }

    Type type() const
    { return mType; }

    SpriteDisplay display() const
    { return mDisplay; }

    QString name() const
    { return mName; }

    QString description() const
    { return mDescription; }

    QList<QString> effects() const
    { return mEffects; }

    int weight() const
    { return mWeight; }

    QMap<BeingGender, SpriteReference *> sprites() const
    { return mSprites; }

    SpriteReference *sprite(BeingGender gender) const
    {
        if (mSprites.contains(gender))
            return mSprites.value(gender);

        return mSprites[GENDER_UNSPECIFIED];
    }

    QString particleFx() const
    { return mParticleFx; }

protected:
    ItemInfo(ItemDB *db, int id) : QObject(db), mId(id) {}

    int mId;
    Type mType;
    SpriteDisplay mDisplay; // Icon
    QString mName;
    QString mDescription;
    QList<QString> mEffects;
    int mWeight;

    QMap<BeingGender, SpriteReference *> mSprites;
    QString mParticleFx;
};

} // namespace Mana

#endif // ITEMDB_H
