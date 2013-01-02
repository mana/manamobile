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

#ifndef NPCDB_H
#define NPCDB_H

#include "spritedef.h"

#include "mana/protocol.h"

#include <QList>
#include <QMap>

class XmlReader;

#define NPC_DB_FILE "npcs.xml"

class QNetworkReply;

namespace Mana {

class NpcDB : public QObject
{
Q_OBJECT

public:
    explicit NpcDB(QObject *parent);

    Q_INVOKABLE void load();

    Q_INVOKABLE void unload();

    bool loaded() const { return mLoaded; }

    const QList<SpriteReference *> getSprites(int id) const
    { return mNpcs.value(id); }

    static NpcDB *instance() { return mInstance; }

signals:
    void npcsChanged();

private slots:
    void fileReady();

private:
    static NpcDB *mInstance;

    bool mLoaded;

    ~NpcDB() { unload(); }

    QMap<int, QList<SpriteReference *> > mNpcs;
};

} // namespace Mana

#endif // NPCDB_H
