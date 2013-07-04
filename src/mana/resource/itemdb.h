/*
 * Mana QML plugin
 * Copyright (C) 2011  Jared Adams 
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

#ifndef ITEMDB_H
#define ITEMDB_H

#include "spritedef.h"

#include "mana/protocol.h"

#include <QList>
#include <QMap>
#include <QVector>

class XmlReader;

#define ITEMS_DB_FILE "items.xml"

class QNetworkReply;

namespace Mana {

class ItemInfo;

class ItemDB : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<const ItemInfo*> items READ items NOTIFY itemsChanged)

public:
    class Stat
    {
    public:
        Stat(QString tag, QString format)
            : tag(tag), format(format)
        {}

        bool operator ==(QString name) const { return tag == name; }

        const QString tag;
        const QString format;
    };

    ItemDB(QObject *parent);

    Q_INVOKABLE void load();
    Q_INVOKABLE void unload();
    bool isLoaded() const { return mLoaded; }

    Q_INVOKABLE const ItemInfo *getInfo(int id) const;

    void setStatsList(QList<Stat> stats);

    QList<const ItemInfo*> items() const;

    static ItemDB *instance() { return mInstance; }

signals:
    void itemsChanged();
    void loaded();

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

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(ItemInfo::Type type READ type CONSTANT)
    Q_PROPERTY(SpriteDisplay display READ display CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QStringList effects READ effects CONSTANT)
    Q_PROPERTY(int weight READ weight CONSTANT)
    Q_PROPERTY(QString particleFx READ particleFx CONSTANT)

public:
    explicit ItemInfo(int id, QObject *parent = 0)
        : QObject(parent), mId(id)
    {}

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

    int id() const
    { return mId; }

    Type type() const { return mType; }
    void setType(Type type) { mType = type; }

    SpriteDisplay display() const { return mDisplay; }
    void setDisplay(const SpriteDisplay &display) { mDisplay = display; }

    QString name() const { return mName; }
    void setName(const QString &name) { mName = name; }

    QString description() const { return mDescription; }
    void setDescription(const QString &description) { mDescription = description; }

    QStringList effects() const { return mEffects; }
    void setEffects(const QStringList &effects) { mEffects = effects; }

    int weight() const { return mWeight; }
    void setWeight(int weight) { mWeight = weight; }

    QMap<BeingGender, QVector<SpriteReference *> > sprites() const
    { return mSprites; }

    QVector<SpriteReference *> sprites(BeingGender gender) const
    {
        if (mSprites.contains(gender))
            return mSprites.value(gender);

        return mSprites[GENDER_UNSPECIFIED];
    }

    void addSprite(BeingGender gender, SpriteReference *sprite)
    { mSprites[gender].append(sprite); }

    QString particleFx() const { return mParticleFx; }
    void setParticleFx(const QString &particleFx) { mParticleFx = particleFx; }

private:
    int mId;
    Type mType;
    SpriteDisplay mDisplay; // Icon
    QString mName;
    QString mDescription;
    QStringList mEffects;
    int mWeight;

    QMap<BeingGender, QVector<SpriteReference *> > mSprites;
    QString mParticleFx;
};

} // namespace Mana

#endif // ITEMDB_H
