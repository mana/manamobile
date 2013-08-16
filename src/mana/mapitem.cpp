/*
 * Mana Mobile
 * Copyright (C) 2010  Thorbjørn Lindeijer
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

#include "mapitem.h"

#include "resourcemanager.h"
#include "tilelayeritem.h"

#include "tiled/isometricrenderer.h"
#include "tiled/map.h"
#include "tiled/orthogonalrenderer.h"
#include "tiled/tilelayer.h"
#include "tiled/tileset.h"
#include "tiled/mapreader.h"

#include "mana/resource/imageresource.h"

#include <QFileInfo>
#include <QImageReader>
#include <QNetworkReply>

#include <QDebug>

using namespace Tiled;
using namespace Mana;

MapItem::MapItem(QQuickItem *parent)
    : QQuickItem(parent)
    , mStatus(Null)
    , mMap(0)
    , mRenderer(0)
{
}

void MapItem::setSource(const QString &source)
{
    if (mSource == source)
        return;

    mSource = source;
    emit sourceChanged(mSource);

    if (isComponentComplete())
        load();
}

void MapItem::setVisibleArea(const QRectF &visibleArea)
{
    mVisibleArea = visibleArea;
    emit visibleAreaChanged();
}

QRectF MapItem::boundingRect() const
{
    if (!mMap)
        return QRectF();

    return QRectF(QPointF(), mRenderer->mapSize());
}

void MapItem::componentComplete()
{
    QQuickItem::componentComplete();
    if (!mSource.isEmpty())
        load();
}

void MapItem::setStatus(Status status)
{
    if (mStatus == status)
        return;

    mStatus = status;
    emit statusChanged();
}

void MapItem::load()
{
    // Clean up ourselves (maybe wait until the map is available?)
    qDeleteAll(mTileLayerItems);
    mTileLayerItems.clear();
    delete mMap;
    delete mRenderer;
    mMap = 0;
    mRenderer = 0;

    // Cancel any previous requests
    foreach (QNetworkReply *reply, mPendingResources) {
        disconnect(reply, SIGNAL(finished()));
        reply->deleteLater();
    }
    mPendingResources.clear();

    foreach (ImageResource *imageResource, mPendingImageResources) {
        disconnect(imageResource, SIGNAL(statusChanged(Resource::Status)),
                   this, SLOT(imageStatusChanged()));
    }
    mPendingImageResources.clear();

    foreach (ImageResource *imageResource, mImageResources)
        imageResource->decRef();
    mImageResources.clear();

    // Request the new map
    QNetworkReply *reply = ResourceManager::instance()->requestFile(mSource);
    mPendingResources.append(reply);
    mMapReply = reply;

    connect(reply, SIGNAL(finished()), this, SLOT(mapFinished()));

    setStatus(Loading);
}

QNetworkReply *MapItem::finishReply()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    mPendingResources.removeOne(reply);
    return reply;
}

void MapItem::checkReady()
{
    if (mStatus == Loading) {
        if (mPendingResources.isEmpty() && mPendingImageResources.isEmpty())
            setStatus(Ready);
    }
}

void MapItem::mapFinished()
{
    QNetworkReply *reply = finishReply();

    // Ignore late replies if the map has changed meanwhile
    if (reply != mMapReply)
        return;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Failed to download map:" << mSource << "\n"
                 << reply->errorString();
        return;
    }

    Tiled::MapReader reader;
    reader.setLazy(true); // Don't have it load external resources immediately

    mMap = reader.readMap(reply, QFileInfo(mSource).path());
    if (!mMap) {
        qDebug() << "Failed to load map:" << mSource << "\n"
                 << reader.errorString();
        return;
    }

    switch (mMap->orientation()) {
    case Map::Isometric:
        mRenderer = new IsometricRenderer(mMap);
        break;
    default:
        mRenderer = new OrthogonalRenderer(mMap);
        break;
    }

    bool seenFringe = false;
    foreach (Layer *layer, mMap->layers()) {
        if (TileLayer *tl = dynamic_cast<TileLayer*>(layer)) {
            if (tl->name().compare(QLatin1String("collision"), Qt::CaseInsensitive) == 0)
                continue;

            TileLayerItem *layerItem = new TileLayerItem(tl, mRenderer, this);
            if (seenFringe)
                layerItem->setZ(65536);
            else if (tl->name().compare(QLatin1String("fringe"), Qt::CaseInsensitive) == 0)
                seenFringe = true;
            mTileLayerItems.append(layerItem);
        }
    }

    const QSize size = mRenderer->mapSize();
    setImplicitSize(size.width(), size.height());

    // Request the external resources that were not loaded yet
    foreach (Tileset *tileset, mMap->tilesets()) {
        if (!tileset->fileName().isEmpty()) {
            ResourceManager *rm = ResourceManager::instance();
            QNetworkReply *reply = rm->requestFile(tileset->fileName());
            mPendingResources.append(reply);
            connect(reply, SIGNAL(finished()), this, SLOT(tilesetFinished()));
        }
        else if (!tileset->imageSource().isEmpty()) {
            requestTilesetImage(tileset);
        }
    }

    checkReady();
}

void MapItem::tilesetFinished()
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

        foreach (Tileset *ts, mMap->tilesets()) {
            if (ts->fileName() == tilesetFilePath) {
                mMap->replaceTileset(ts, tileset);
                delete ts;
                break;
            }
        }

        // The above tileset replacement may change the tile layer draw margins
        foreach (TileLayerItem *layerItem, mTileLayerItems)
            layerItem->updateVisibleTiles();
    }

    checkReady();
}

void MapItem::imageStatusChanged()
{
    ImageResource *imgRes = static_cast<ImageResource*>(sender());
    mPendingImageResources.remove(imgRes);

    // Trigger a repaint of the tile layer items
    if (imgRes->status() == Resource::Ready) {
        foreach (TileLayerItem *layerItem, mTileLayerItems)
            layerItem->update();
    } else {
        qWarning() << "Error loading tileset image" << imgRes->url();
    }

    checkReady();
}

void MapItem::requestTilesetImage(Tileset *tileset)
{
    Q_ASSERT(!tileset->imageSource().isEmpty());

    ResourceManager *rm = ResourceManager::instance();
    ImageResource *imgRes = rm->requestImage(tileset->imageSource());
    mImageResources.insert(tileset, imgRes);

    if (imgRes->status() == Resource::Loading) {
        mPendingImageResources.insert(imgRes);
        connect(imgRes, SIGNAL(statusChanged(Resource::Status)),
                this, SLOT(imageStatusChanged()));
    } else {
        foreach (TileLayerItem *layerItem, mTileLayerItems)
            layerItem->update();
    }
}
