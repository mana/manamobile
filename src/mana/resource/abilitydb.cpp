/*
 * Mana QML plugin
 * Copyright (C) 2013  Erik Schilling
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

#include "abilitydb.h"

#include "mana/resourcemanager.h"

#include "mana/xmlreader.h"

#include <QNetworkReply>

#define ABILITIES_DB_FILE "abilities.xml"

using namespace Mana;

AbilityDB *AbilityDB::mInstance = 0;

AbilityDB::AbilityDB(QObject *parent)
    : QObject(parent)
    , mLoaded(false)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

void AbilityDB::load()
{
    if (mLoaded)
        return;

    QNetworkReply *reply = ResourceManager::instance()->requestFile(
                ABILITIES_DB_FILE);

    connect(reply, SIGNAL(finished()), this, SLOT(fileReady()));
}

void AbilityDB::unload()
{
    qDeleteAll(mAbilities);
    mAbilities.clear();

    mLoaded = false;
    emit abilitiesChanged();
}

static AbilityInfo::AbilityTargetType targetTypeFromString(const QString &value)
{
    if (value == "being")
        return AbilityInfo::TARGET_BEING;
    else if (value == "point")
        return AbilityInfo::TARGET_POINT;
    else if (value == "direction")
        return AbilityInfo::TARGET_DIRECTION;

    return AbilityInfo::TARGET_BEING;
}

void AbilityDB::fileReady()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    XmlReader xml(reply);

    if (!xml.readNextStartElement() || xml.name() != "abilities") {
        qWarning() << "Error loading abilities.xml";
        return;
    }

    while (xml.readNextStartElement()) {
        if (xml.name() != "ability")
            xml.skipCurrentElement();

        const QXmlStreamAttributes atts = xml.attributes();
        unsigned id = atts.value("id").toInt();
        QString name = atts.value("name").toString();
        QString useAction = atts.value("useaction").toString();

        QString targetAsString = atts.value("target").toString();
        AbilityInfo::AbilityTargetType targetType =targetTypeFromString(
                    targetAsString);

        if (id == 0 || name.isEmpty()) {
            qWarning() << "Bad ability at " << xml.lineNumber()
                       << " skipping...";
            xml.skipCurrentElement();
            continue;
        }

        if (mAbilities.find(id) != mAbilities.end()) {
            qWarning() << "Duplicate ability id at " << xml.lineNumber()
                       << " skipping...";
            xml.skipCurrentElement();
            continue;
        }

        AbilityInfo *info = new AbilityInfo(id, this);
        info->setName(name);
        info->setTargetType(targetType);
        info->setUseAction(useAction);

        xml.skipCurrentElement();

        mAbilities[id] = info;
    }

    mLoaded = true;
    emit abilitiesChanged();
}
