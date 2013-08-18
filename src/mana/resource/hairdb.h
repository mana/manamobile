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

#ifndef HAIRDB_H
#define HAIRDB_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QQmlListProperty>

#include "spritedef.h"

#include "mana/protocol.h"

class QNetworkReply;

namespace Mana {

class HairInfo;

class HairDB : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Mana::HairInfo> hairs READ hairs NOTIFY hairsChanged)

    friend class ItemDB;

public:
    HairDB(QObject *parent);

    Q_INVOKABLE void load();
    Q_INVOKABLE void unload();
    bool isLoaded() const { return mLoaded; }

    Q_INVOKABLE const Mana::HairInfo *getInfo(int id) const
    { return mHairMap[id]; }

    void setInfo(int id, HairInfo *info);

    QQmlListProperty<HairInfo> hairs();

    static HairDB *instance() { return mInstance; }

signals:
    void hairsChanged();
    void loaded();

private:
    static int hairs_count(QQmlListProperty<HairInfo> *);
    static HairInfo *hairs_at(QQmlListProperty<HairInfo> *, int index);

    static HairDB *mInstance;

    bool mLoaded;
    ~HairDB() { unload(); }

    QMap<int, HairInfo *> mHairMap;
    QList<HairInfo *> mHairList;
};

class HairInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:
    HairInfo(int id, QObject *parent) : QObject(parent), mId(id) {}

    int id() const
    { return mId; }

    QString name() const { return mName; }
    void setName(const QString &name) { mName = name; }

    QMap<BeingGender, SpriteReference *> sprites() const
    { return mSprites; }

    SpriteReference *sprite(BeingGender gender) const
    {
        if (mSprites.contains(gender))
            return mSprites.value(gender);

        return mSprites[GENDER_UNSPECIFIED];
    }

    void setSprite(BeingGender gender, SpriteReference *sprite)
    { mSprites[gender] = sprite; }

private:
    int mId;
    QString mName;
    QMap<BeingGender, SpriteReference *> mSprites;
};

} // namespace Mana

Q_DECLARE_METATYPE(Mana::HairInfo*)

#endif // HAIRDB_H
