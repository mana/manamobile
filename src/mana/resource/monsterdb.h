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

#ifndef MONSTERDB_H
#define MONSTERDB_H

#include "spritedef.h"

#include "mana/protocol.h"

#include <QList>
#include <QMap>

class XmlReader;

#define MONSTERS_DB_FILE "monsters.xml"

class QNetworkReply;

namespace Mana {

class MonsterInfo;

class MonsterDB : public QObject
{
Q_OBJECT

Q_PROPERTY(QList<const MonsterInfo*> monsters READ monsters NOTIFY monstersChanged)

friend class MonsterInfo;

public:
    MonsterDB(QObject *parent);

    Q_INVOKABLE void load();

    Q_INVOKABLE void unload();

    Q_INVOKABLE const MonsterInfo *getInfo(int id) const
    { return mMonsters.value(id); }

    bool loaded() const { return mLoaded; }

    QList<const MonsterInfo*> monsters() const
    { return mMonsters.values(); }

    static MonsterDB *instance() { return mInstance; }

signals:
    void monstersChanged();

private slots:
    void fileReady();

private:
    static MonsterDB *mInstance;

    ~MonsterDB() { unload(); }

    bool mLoaded;
    QMap<int, const MonsterInfo *> mMonsters;
};

class MonsterInfo : public QObject
{
    Q_OBJECT
    friend class MonsterDB;

    Q_PROPERTY(int id READ id CONSTANT)

    Q_PROPERTY(QString name READ name CONSTANT)

public:
    int id() const
    { return mId; }

    QString name() const
    { return mName; }

    QList<SpriteReference *> sprites() const
    { return mSprites; }

protected:
    MonsterInfo(int id, QString name) : QObject(), mId(id), mName(name) {}

    int mId;
    QString mName;

    QList<SpriteReference *> mSprites;
};

} // namespace Mana

#endif // MONSTERDB_H
