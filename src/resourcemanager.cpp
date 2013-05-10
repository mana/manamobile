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

#include <QDateTime>
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QNetworkConfigurationManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

#include "mana/xmlreader.h"

#include "mana/resource/pixmapresource.h"
#include "mana/resource/resource.h"
#include "mana/resource/spritedef.h"

ResourceManager *ResourceManager::mInstance;

// Time in seconds that a currently unused resource should stay in cache
static const int CACHE_TIME = 60;

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
#if QT_VERSION >= 0x050000
    QString cacheLocation =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#else
    QString cacheLocation =
            QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#endif

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

        mPathsLoaded = true;
        emit pathsLoadedChanged();
        return;
    }

    XmlReader xml(reply);

    if (!xml.readNextStartElement() || xml.name() != "configuration") {
        qWarning() << "Error loading paths.xml";

        mPathsLoaded = true;
        emit pathsLoadedChanged();
        return;
    }

    while (xml.readNextStartElement()) {
        if (xml.name() == "option") {
            const QXmlStreamAttributes atts = xml.attributes();
            const QStringRef name = atts.value("name");
            const QStringRef value = atts.value("value");

            if (!name.isEmpty())
                mPaths[name.toString()] = value.toString();

            xml.skipCurrentElement();
        } else {
            xml.readUnknownElement();
        }
    }

    mPathsLoaded = true;
    emit pathsLoadedChanged();
}

void ResourceManager::setDataUrl(const QString &url)
{
    if (mDataUrl == url)
        return;

    mPaths.clear();
    mPathsLoaded = false;
    emit pathsLoadedChanged();

    mDataUrl = url;
    emit dataUrlChanged();

    // Load the paths.xml if available
    QNetworkReply *reply = requestFile("paths.xml");
    connect(reply, SIGNAL(finished()), this, SLOT(pathsFileFinished()));
}

QNetworkReply *ResourceManager::requestFile(const QString &fileName)
{
    QNetworkRequest request(QUrl(mDataUrl).resolved(fileName));
    request.setAttribute(requestedFileAttribute(), fileName);

    qDebug() << "Retrieving" << request.url();

    return mNetworkAccessManager.get(request);
}

void ResourceManager::removeResource(Mana::Resource *resource)
{
    Q_ASSERT(resource->refCount() == 0);
    Q_ASSERT(mResources.contains(resource->path()));

    mResources.remove(resource->path());
    delete resource;
}

void ResourceManager::cleanUpResources()
{
    unsigned releaseTime = QDateTime::currentMSecsSinceEpoch() - CACHE_TIME;

    foreach (Mana::Resource *resource, mResources)
        if (resource->refCount() == 0 && resource->releaseTime() < releaseTime)
            removeResource(resource);
}

Mana::SpriteDefinition *ResourceManager::requestSpriteDefinition(
        const QString &path, int variant)
{
    Mana::SpriteDefinition *sprite = 0;

    const QString &spritePrefix = spritePath();

    // Check if the sprite is already cached
    QMap<QString, Mana::Resource *>::iterator it =
            mResources.find(spritePrefix + path);

    if (it != mResources.end()) {
        sprite = static_cast<Mana::SpriteDefinition *>(it.value());
    } else {
        sprite = new Mana::SpriteDefinition(this, spritePrefix + path, variant);
        mResources.insert(sprite->path(), sprite);
    }
    sprite->incRef();
    return sprite;
}

Mana::PixmapResource *ResourceManager::requestPixmap(const QString &path)
{
    Mana::PixmapResource *pixmap = 0;

    // Check if the sprite is already cached
    QMap<QString, Mana::Resource *>::iterator it = mResources.find(path);
    if (it != mResources.end()) {
        pixmap = static_cast<Mana::PixmapResource *>(it.value());
    } else {
        pixmap = new Mana::PixmapResource(path, this);
        mResources.insert(path, pixmap);
    }
    pixmap->incRef();
    return pixmap;
}
