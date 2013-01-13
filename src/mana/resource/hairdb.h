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

#ifndef HAIRDB_H
#define HAIRDB_H

#include <QList>
#include <QMap>
#include <QObject>

#include "spritedef.h"

#include "mana/protocol.h"

class QNetworkReply;

namespace Mana {

class HairInfo;

class HairDB : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<HairInfo *> hairs READ hairs NOTIFY hairsChanged)

    friend class ItemDB;

public:
    HairDB(QObject *parent);

    Q_INVOKABLE void load();
    Q_INVOKABLE void unload();
    bool isLoaded() const { return mLoaded; }

    Q_INVOKABLE const HairInfo *getInfo(int id) const
    { return mHairs[id]; }


    QList<HairInfo *> hairs() const { return mHairs.values(); }

    static HairDB *instance() { return mInstance; }

signals:
    void hairsChanged();
    void loaded();

private:
    static HairDB *mInstance;

    bool mLoaded;
    ~HairDB() { unload(); }

    QMap<int, HairInfo *> mHairs;
};

class HairInfo : public QObject
{
    Q_OBJECT
    friend class ItemDB;

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:

    int id() const
    { return mId; }

    QString name() const
    { return mName; }

    QMap<BeingGender, SpriteReference *> sprites() const
    { return mSprites; }

    SpriteReference *sprite(BeingGender gender) const
    {
        if (mSprites.contains(gender))
            return mSprites.value(gender);

        return mSprites[GENDER_UNSPECIFIED];
    }

protected:
    HairInfo(int id, QObject *parent) : QObject(parent), mId(id) {}

    int mId;
    QString mName;
    QMap<BeingGender, SpriteReference *> mSprites;
};

} // namespace Mana

#endif // HAIRDB_H
