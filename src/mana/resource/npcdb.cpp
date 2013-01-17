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

#include "npcdb.h"

#include "../../resourcemanager.h"
#include "../xmlreader.h"

#include "hairdb.h"
#include "racedb.h"

#include <QDebug>
#include <QNetworkReply>

using namespace Mana;

NpcDB *NpcDB::mInstance = 0;

NpcDB::NpcDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

void NpcDB::load()
{
    if (mLoaded)
        return;

    QNetworkReply *reply = ResourceManager::instance()->requestFile(NPC_DB_FILE);

    connect(reply, SIGNAL(finished()), this, SLOT(fileReady()));
}

void NpcDB::unload()
{
    mNpcs.clear();

    mLoaded = false;
    emit npcsChanged();
}

void NpcDB::fileReady()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    XmlReader xml(reply);

    if (!xml.readNextStartElement() || xml.name() != "npcs") {
        qWarning() << "Error loading npcs.xml";
        return;
    }

    while (xml.readNextStartElement()) {
        if (xml.name() == "npc") {
            const QXmlStreamAttributes atts = xml.attributes();
            int id = atts.value("id").toString().toInt();

            if (!id) {
                qWarning() << "Bad NPC id at line " << xml.lineNumber();
                xml.skipCurrentElement();
                continue;
            }

            QList<SpriteReference *> sprites;
            while (xml.readNextStartElement()) {
                if (xml.name() == "sprite")
                    sprites.append(SpriteReference::readSprite(xml));
                else
                    xml.readUnknownElement();
            }
            mNpcs[id] = sprites;
        } else {
            xml.readUnknownElement();
        }
    }

    mLoaded = true;
    emit npcsChanged();
    emit loaded();
}
