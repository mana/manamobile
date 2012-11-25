/*
 *  Mana Mobile
 *  Copyright (C) 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *  Copyright (C) 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
 *
 *  This file is part of Mana Mobile.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "resourcemanager.h"

#include <QDesktopServices>
#include <QNetworkConfigurationManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

#include "mana/xmlreader.h"

ResourceManager *ResourceManager::mInstance;

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent)
    , mPathsLoaded(false)
{
    // TODO: This takes about 400 ms on my system. Doing it here prevents
    // experiencing this hickup later on when the the network access manager is
    // used for the first time. Even on startup it's ugly though, so hopefully
    // there's a way to avoid it completely...
    QNetworkConfigurationManager manager;
    mNetworkAccessManager.setConfiguration(manager.defaultConfiguration());

    // Use a disk cache to avoid re-downloading data all the time
    QString cacheLocation =
            QDesktopServices::storageLocation(QDesktopServices::CacheLocation);

    if (!cacheLocation.isEmpty()) {
        cacheLocation += QLatin1String("/httpCache");

        QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
        diskCache->setCacheDirectory(cacheLocation);
        mNetworkAccessManager.setCache(diskCache);
    } else {
        qWarning() << "CacheLocation is not supported on this platform, "
                      "no disk cache is used!";
    }

    Q_ASSERT(!mInstance);
    mInstance = this;
}

void ResourceManager::pathsFileFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    // 404 is fine since we will just pick defaults then
    if (!(reply->error() == QNetworkReply::NoError ||
            reply->error() == QNetworkReply::ContentNotFoundError)) {
        qDebug() << "Failed to download paths.xml:\n"
                 << reply->errorString();
        mPathsLoaded = false;
        return;
    }

    XmlReader xml(reply);

    while (!xml.atEnd())
    {
        xml.readNext();
        if (!xml.isStartElement())
            continue;

        if (xml.name() == "option") {
            QString key = xml.attribute("name");
            QString value = xml.attribute("value");

            if (key.isEmpty()) {
                qDebug() << Q_FUNC_INFO
                         << "Invalid value for the attribute \"name\"!";
                continue;
            }

            mPaths[key] = value;
        }
    }

    mPathsLoaded = true;
    emit pathsLoaded();
}

void ResourceManager::setDataUrl(const QString &url)
{
    if (mDataUrl == url)
        return;

    mDataUrl = url;
    emit dataUrlChanged();

    // Load the paths.xml if available
    QNetworkReply *reply = requestFile("paths.xml");
    connect(reply, SIGNAL(finished()), this, SLOT(pathsFileFinished()));
}

QNetworkReply *ResourceManager::requestFile(const QString &fileName)
{
    qDebug() << Q_FUNC_INFO << fileName;

    const QNetworkRequest request(mDataUrl + fileName);
    QNetworkReply *reply = mNetworkAccessManager.get(request);
    return reply;
}
