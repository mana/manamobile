/*
 *  Mana Mobile
 *  Copyright (C) 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#include "mapitem.h"

#include "resourcemanager.h"
#include "tilelayeritem.h"

#include "tiled/isometricrenderer.h"
#include "tiled/map.h"
#include "tiled/orthogonalrenderer.h"
#include "tiled/tilelayer.h"
#include "tiled/tileset.h"
#include "tiled/mapreader.h"

#include <QFileInfo>
#include <QImageReader>
#include <QNetworkReply>

using namespace Tiled;
using namespace Tiled::Internal;

MapItem::MapItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
    , mStatus(Null)
    , mMap(0)
    , mRenderer(0)
{
    qDebug() << Q_FUNC_INFO;
    setFlag(ItemHasNoContents);
}

void MapItem::setSource(const QString &source)
{
    qDebug() << Q_FUNC_INFO << source << isComponentComplete();
    if (mSource == source)
        return;

    mSource = source;
    emit sourceChanged(mSource);

    if (isComponentComplete())
        load();
}

QRectF MapItem::boundingRect() const
{
    if (!mMap)
        return QRectF();

    return QRectF(QPointF(), mRenderer->mapSize());
}

void MapItem::componentComplete()
{
    QDeclarativeItem::componentComplete();
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
    qDebug() << Q_FUNC_INFO << mSource;

    // Clean up ourselves (maybe wait until the map is available?)
    qDeleteAll(childItems());
    delete mMap;
    delete mRenderer;
    mMap = 0;
    mRenderer = 0;

    // Cancel any previous requests
    foreach (QNetworkReply *reply, mPendingResources)
        reply->deleteLater();
    mPendingResources.clear();

    // Request the new map
    QNetworkReply *reply = ResourceManager::instance()->requestFile(mSource);
    mPendingResources.append(reply);

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
    if (mStatus == Loading && mPendingResources.isEmpty())
        setStatus(Ready);
}

void MapItem::mapFinished()
{
    QNetworkReply *reply = finishReply();

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

    int layerIndex = 0;
    foreach (Layer *layer, mMap->layers()) {
        if (TileLayer *tl = dynamic_cast<TileLayer*>(layer)) {
            QGraphicsItem *layerItem = new TileLayerItem(tl, mRenderer, this);
            layerItem->setZValue(layerIndex);
            ++layerIndex;
        }
    }

    const QSize size = mRenderer->mapSize();
    setImplicitWidth(size.width());
    setImplicitHeight(size.height());

    // Request the external resources that were not loaded yet
    ResourceManager *rm = ResourceManager::instance();
    foreach (Tileset *tileset, mMap->tilesets()) {
        if (!tileset->fileName().isEmpty()) {
            QNetworkReply *reply = rm->requestFile(tileset->fileName());
            mPendingResources.append(reply);
            connect(reply, SIGNAL(finished()), this, SLOT(tilesetFinished()));
        }
        else if (!tileset->imageSource().isEmpty()) {
            QNetworkReply *reply = rm->requestFile(tileset->imageSource());
            mPendingResources.append(reply);
            connect(reply, SIGNAL(finished()), this, SLOT(imageFinished()));
        }
    }

    checkReady();
}

void MapItem::tilesetFinished()
{
    QNetworkReply *reply = finishReply();

    const QNetworkRequest request = reply->request();
    const QString tilesetFilePath =
            request.attribute(ResourceManager::requestedFileAttribute()).toString();
    Q_ASSERT(!tilesetFilePath.isEmpty());

    ResourceManager *rm = ResourceManager::instance();

    const int lastSlashPos = tilesetFilePath.lastIndexOf(QLatin1Char('/'));
    const QString tilesetPath = tilesetFilePath.left(lastSlashPos);

    Tiled::MapReader reader;
    reader.setLazy(true); // Don't have it load the tileset image immediately

    Tiled::Tileset *tileset = reader.readTileset(reply, tilesetPath);
    if (!tileset) {
        qDebug() << "Error reading tileset:" << tilesetFilePath << "\n"
                 << reader.errorString();
    } else {
        QNetworkReply *reply = rm->requestFile(tileset->imageSource());
        mPendingResources.append(reply);
        connect(reply, SIGNAL(finished()), this, SLOT(imageFinished()));

        foreach (Tileset *ts, mMap->tilesets()) {
            if (ts->fileName() == tilesetFilePath) {
                mMap->replaceTileset(ts, tileset);
                delete ts;
                break;
            }
        }
    }

    checkReady();
}

void MapItem::imageFinished()
{
    QNetworkReply *reply = finishReply();

    const QNetworkRequest request = reply->request();
    const QString requestedFile =
            request.attribute(ResourceManager::requestedFileAttribute()).toString();
    Q_ASSERT(!requestedFile.isEmpty());

    // TODO: Remove hack here to pass image type based on extension. If we
    // don't do this, Qt seems to query all image plugins and the 'ras' one
    // will warn about not supporting sequential devices...
    QImageReader reader(reply, QFileInfo(requestedFile).suffix().toLatin1());
    const QImage image = reader.read();

    if (image.isNull()) {
        qDebug() << "Failed to read image!";
        return;
    }

    foreach (Tileset *tileset, mMap->tilesets()) {
        if (requestedFile == tileset->imageSource())
            tileset->loadFromImage(image, tileset->imageSource());
    }

    // Trigger a repaint of the tile layer items
    foreach (QGraphicsItem *child, childItems())
        child->update();

    checkReady();
}
