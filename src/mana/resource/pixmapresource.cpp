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

#include "pixmapresource.h"

#include <QDebug>
#include <QNetworkReply>

#include "resource.h"
#include "resourcemanager.h"

using namespace Mana;

PixmapResource::PixmapResource(const QString &filePath, QObject *parent)
    : Resource(parent)
    , mPixmap(0)
{
    int pos = filePath.indexOf(QLatin1Char('|'));

    QString palettes;
    if (pos != -1)
        palettes = filePath.right(filePath.length() - pos);

    // TODO: Dye

    const QString filePathWithoutDye = filePath.left(pos);
    QNetworkReply *reply = ResourceManager::instance()->requestFile(
                filePathWithoutDye);
    connect(reply, SIGNAL(finished()), this, SLOT(imageFinished()));
    setStatus(Loading);
}

PixmapResource::~PixmapResource()
{
    delete mPixmap;
}

void PixmapResource::imageFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Failed to download image:" << reply->url() << "\n"
                 << reply->errorString();
        setStatus(Error);
        return;
    }

    reply->deleteLater();
    mPixmap = new QPixmap();
    bool success = mPixmap->loadFromData(reply->readAll());
    setStatus(success ? Ready : Error);
}
