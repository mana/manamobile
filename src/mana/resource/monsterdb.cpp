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

#include "monsterdb.h"

#include "mana/resourcemanager.h"
#include "mana/xmlreader.h"

#include <QNetworkReply>

using namespace Mana;

MonsterDB *MonsterDB::mInstance = 0;

MonsterDB::MonsterDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

void MonsterDB::load()
{
    if (mLoaded)
        return;

    ResourceManager *resourceManager = ResourceManager::instance();
    QNetworkReply *reply = resourceManager->requestFile(MONSTERS_DB_FILE);

    connect(reply, SIGNAL(finished()), this, SLOT(fileReady()));
}

void MonsterDB::unload()
{
    qDeleteAll(mMonsters);
    mMonsters.clear();

    mLoaded = false;
    emit monstersChanged();
}

void MonsterDB::fileReady()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    XmlReader xml(reply);

    if (!xml.readNextStartElement() || xml.name() != "monsters") {
        xml.raiseError(tr("Not an monster database."));
        return;
    }

    while (xml.readNextStartElement()) {
        if (xml.name() != "monster") {
            xml.readUnknownElement();
            continue;
        }

        const QXmlStreamAttributes attr = xml.attributes();

        const int id = attr.value("id").toString().toInt();
        const QString name = attr.value("name").toString();

        if (!id) {
            if (name.isEmpty())
                xml.raiseError(tr("Invalid monster id"));
            else
                xml.raiseError(tr("Invalid id for monster %1").arg(id));
            return;
        }
        if (name.isEmpty()) {
            xml.raiseError(tr("Empty name for monster id %1").arg(id));
            return;
        }

        MonsterInfo *info = new MonsterInfo(id, name);

        while (xml.readNextStartElement()) {
            if (xml.name() == "sprite") {
                SpriteReference *sprite = SpriteReference::readSprite(xml,
                                                                      info);
                info->mSprites.append(sprite);
            } else {
                xml.skipCurrentElement();
            }
        }
        mMonsters[id] = info;
    }

    mLoaded = true;
    emit monstersChanged();
    emit loaded();
}
