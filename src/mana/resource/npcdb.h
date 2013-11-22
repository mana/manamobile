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
    bool isLoaded() const { return mLoaded; }

    const QList<SpriteReference *> getSprites(int id) const
    { return mNpcs.value(id); }

    static NpcDB *instance() { return mInstance; }

signals:
    void npcsChanged();
    void loaded();

private slots:
    void fileReady();

private:
    static NpcDB *mInstance;

    bool mLoaded;

    QMap<int, QList<SpriteReference *> > mNpcs;
};

} // namespace Mana

#endif // NPCDB_H
