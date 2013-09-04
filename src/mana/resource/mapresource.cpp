/*
 * Mana QML plugin
 * Copyright (C) 2010-2013  Thorbjørn Lindeijer
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

#include "mapresource.h"

#include "mana/resourcemanager.h"

#include "mana/resource/imageresource.h"

#include "tiled/map.h"
#include "tiled/mapreader.h"
#include "tiled/tilelayer.h"
#include "tiled/tileset.h"

#include <QDebug>
#include <QFileInfo>
#include <QNetworkReply>

namespace Mana {

MapResource::MapResource(const QUrl &url,
                         const QString &path,
                         QObject *parent)
    : Resource(url, parent)
    , mPath(QFileInfo(path).path())
    , mMap(0)
    , mCollisionLayer(0)
{
    ResourceManager *resourceManager = ResourceManager::instance();
    QNetworkReply *reply = resourceManager->requestFile(url);
    connect(reply, SIGNAL(finished()), this, SLOT(mapFinished()));
    setStatus(Loading);
}

void MapResource::mapFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Failed to download map:" << reply->url() << "\n"
                 << reply->errorString();
        setStatus(Error);
        return;
    }

    Tiled::MapReader reader;
    reader.setLazy(true); // Don't have it load external resources immediately

    mMap = reader.readMap(reply, mPath);
    if (!mMap) {
        qDebug() << "Failed to load map:" << reply->url() << "\n"
                 << reader.errorString();
        setStatus(Error);
        return;
    }

    foreach (Tiled::Layer *layer, mMap->layers()) {
        if (Tiled::TileLayer *tl = dynamic_cast<Tiled::TileLayer*>(layer)) {
            if (tl->name().compare(QLatin1String("collision"), Qt::CaseInsensitive) == 0) {
                mCollisionLayer = tl;
                continue;
            }
        }
    }

    // Request the external tilesets that were not loaded yet
    foreach (Tiled::Tileset *tileset, mMap->tilesets()) {
        if (!tileset->fileName().isEmpty()) {
            ResourceManager *rm = ResourceManager::instance();
            QNetworkReply *reply = rm->requestFile(tileset->fileName());
            mPendingResources.append(reply);
            connect(reply, SIGNAL(finished()), this, SLOT(tilesetFinished()));
        } else if (!tileset->imageSource().isEmpty()) {
            requestTilesetImage(tileset);
        }
    }

    checkReady();
}

void MapResource::tilesetFinished()
{
    QNetworkReply *reply = finishReply();
    if (!mMap) // Ignore request when we already changed map again
        return;

    const QNetworkRequest request = reply->request();
    const QString tilesetFilePath =
            request.attribute(ResourceManager::requestedFileAttribute()).toString();
    Q_ASSERT(!tilesetFilePath.isEmpty());

    const int lastSlashPos = tilesetFilePath.lastIndexOf(QLatin1Char('/'));
    const QString tilesetPath = tilesetFilePath.left(lastSlashPos);

    Tiled::MapReader reader;
    reader.setLazy(true); // Don't have it load the tileset images immediately

    Tiled::Tileset *tileset = reader.readTileset(reply, tilesetPath);
    if (!tileset) {
        qDebug() << "Error reading tileset:" << tilesetFilePath << "\n"
                 << reader.errorString();
    } else {
        if (!tileset->imageSource().isEmpty())
            requestTilesetImage(tileset);

        foreach (Tiled::Tileset *ts, mMap->tilesets()) {
            if (ts->fileName() == tilesetFilePath) {
                mMap->replaceTileset(ts, tileset);
                delete ts;
                break;
            }
        }
    }

    checkReady();
}

void MapResource::imageStatusChanged()
{
    ImageResource *imgRes = static_cast<ImageResource*>(sender());
    mPendingImageResources.remove(imgRes);

    if (imgRes->status() != Resource::Ready)
        qWarning() << "Error loading tileset image" << imgRes->url();

    checkReady();
}

QNetworkReply *MapResource::finishReply()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    mPendingResources.removeOne(reply);
    return reply;
}

void MapResource::checkReady()
{
    if (status() == Loading) {
        if (mPendingResources.isEmpty() && mPendingImageResources.isEmpty())
            setStatus(Ready);
    }
}

void MapResource::requestTilesetImage(Tiled::Tileset *tileset)
{
    Q_ASSERT(!tileset->imageSource().isEmpty());

    ResourceManager *rm = ResourceManager::instance();
    ImageResource *imgRes = rm->requestImage(tileset->imageSource());
    mImageResources.insert(tileset, imgRes);

    if (imgRes->status() == Resource::Loading) {
        mPendingImageResources.insert(imgRes);
        connect(imgRes, SIGNAL(statusChanged(Resource::Status)),
                this, SLOT(imageStatusChanged()));
    }
}

} // namespace Mana
