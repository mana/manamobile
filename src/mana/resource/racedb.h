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

#ifndef RACEDB_H
#define RACEDB_H

#include <QList>
#include <QMap>
#include <QObject>

#include "spritedef.h"

#include "mana/protocol.h"

class QNetworkReply;

namespace Mana {

class RaceInfo;

class RaceDB : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<RaceInfo *> races READ races NOTIFY racesChanged)

    friend class ItemDB;

public:
    RaceDB(QObject *parent);

    Q_INVOKABLE void load();
    Q_INVOKABLE void unload();
    bool isLoaded() const { return mLoaded; }

    Q_INVOKABLE const RaceInfo *getInfo(int id) const
    { return mRaces[id]; }

    void setInfo(int id, RaceInfo *info)
    { mRaces[id] = info; }

    QList<RaceInfo *> races() { return mRaces.values(); }

    static RaceDB *instance() { return mInstance; }

signals:
    void racesChanged();
    void loaded();

private:
    static RaceDB *mInstance;

    bool mLoaded;
    ~RaceDB() { unload(); }

    QMap<int, RaceInfo *> mRaces;
};

class RaceInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:
    RaceInfo(int id, QObject *parent = 0) : QObject(parent), mId(id) {}

    int id() const { return mId; }

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

#endif // RACEDB_H
