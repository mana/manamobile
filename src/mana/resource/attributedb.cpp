/*
 * manalib
 * Copyright 2013, Erik Schilling <ablu.erikschilling@googlemail.com>
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

#include "attributedb.h"

#include "mana/resourcemanager.h"
#include "mana/xmlreader.h"

#include <QNetworkReply>

#define ATTRIBUTE_DB_FILE "attributes.xml"

using namespace Mana;

AttributeDB *AttributeDB::mInstance = 0;

AttributeDB::AttributeDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

void AttributeDB::load()
{
    if (mLoaded)
        return;

    QNetworkReply *reply = ResourceManager::instance()->requestFile(
                ATTRIBUTE_DB_FILE);

    connect(reply, SIGNAL(finished()), this, SLOT(fileReady()));
}

void AttributeDB::unload()
{
    qDeleteAll(mAttributes);
    mAttributes.clear();

    mLoaded = false;
    emit attributesChanged();
}

void AttributeDB::fileReady()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    XmlReader xml(reply);

    if (!xml.readNextStartElement() || xml.name() != "attributes") {
        qWarning() << "Error loading " << ATTRIBUTE_DB_FILE;
        return;
    }

    while (xml.readNextStartElement()) {
        if (xml.name() == "attribute") {
            const QXmlStreamAttributes atts = xml.attributes();
            int id = atts.value("id").toString().toInt();
            QString name = atts.value("name").toString();
            QString description = atts.value("desc").toString();
            bool modifiable = atts.value("modifiable", "false").toString()
                    == "true";
            qreal min = atts.value("minimum").toString().toDouble();
            qreal max = atts.value("maximum").toString().toDouble();

            if (!id) {
                qWarning() << "Bad attribute id at line " << xml.lineNumber();
                xml.skipCurrentElement();
                continue;
            }

            if (mAttributes.contains(id)) {
                qWarning() << "Duplicate attribute id (" << id << ") at line "
                              << xml.lineNumber();
                xml.skipCurrentElement();
                continue;
            }

            xml.skipCurrentElement();

            mAttributes[id] = new AttributeInfo(id, name, description,
                                                modifiable, min, max,
                                                this);
        } else {
            xml.skipCurrentElement();
        }
    }

    mLoaded = true;
    emit attributesChanged();
    emit loaded();
}
