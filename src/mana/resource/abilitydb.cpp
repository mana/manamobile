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
        if (xml.name() != "ability-category")
            xml.skipCurrentElement();

        const QXmlStreamAttributes categoryAtts = xml.attributes();
        QString categoryName = categoryAtts.value("name").toString();

        while (xml.readNextStartElement()) {
            if (xml.name() != "ability")
                xml.skipCurrentElement();

            const QXmlStreamAttributes atts = xml.attributes();
            unsigned id = atts.value("id").toString().toInt();
            QString name = categoryAtts.value("name").toString();
            bool rechargeable = atts.value("rechargeable") == "true" ?
                        true : false;
            unsigned neededPoints = atts.value("needed").toString().toInt();
            unsigned rechargeSpeed =
                    atts.value("rechargespeed").toString().toInt();
            QString useAction = atts.value("useaction").toString();

            QString targetAsString = atts.value("target").toString();
            AbilityInfo::AbilityTargetType targetType;
            if (targetAsString == "being")
                targetType = AbilityInfo::TARGET_BEING;
            else
                targetType = AbilityInfo::TARGET_POINT;

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
            info->setCategory(categoryName);
            info->setRechargable(rechargeable);
            info->setNeededPoints(neededPoints);
            info->setRechargeSpeed(rechargeSpeed);
            info->setTargetType(targetType);
            info->setUseAction(useAction);


            xml.skipCurrentElement();

            mAbilities[id] = info;
        }
    }

    mLoaded = true;
    emit abilitiesChanged();
}
