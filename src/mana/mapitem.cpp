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

#include "mana/resource/mapresource.h"

#include <cmath>

using namespace Mana;

MapItem::MapItem(QQuickItem *parent)
    : QQuickItem(parent)
    , mMapResource(0)
    , mHideCollisionLayer(true)
    , mRenderer(0)
    , mFringeLayer(0)
{
}

void MapItem::setMapResource(MapResource *map)
{
    if (mMapResource == map)
        return;

    if (mMapResource)
        mMapResource->disconnect(this);

    mMapResource = map;
    emit statusChanged();
    emit mapChanged();

    if (mMapResource) {
        connect(mMapResource, SIGNAL(statusChanged(Resource::Status)),
                SLOT(mapStatusChanged()));
    }

    refresh();
}

MapItem::Status MapItem::status() const
{
    return mMapResource ? static_cast<Status>(mMapResource->status()) : Null;
}

void MapItem::setVisibleArea(const QRectF &visibleArea)
{
    mVisibleArea = visibleArea;
    emit visibleAreaChanged();

    updateFringeLayer();
}

/**
 * Determines the rectangle of visible tiles of the given tile \a layer, based
 * on the visible area of this MapItem instance.
 *
 * Only works for orthogonal maps.
 */
QRect MapItem::visibleTileArea(const Tiled::TileLayer *layer) const
{
    const Tiled::Map *map = mMapResource->map();

    const int tileWidth = map->tileWidth();
    const int tileHeight = map->tileHeight();

    QMargins drawMargins = layer->drawMargins();
    drawMargins.setTop(drawMargins.top() - tileHeight);
    drawMargins.setRight(drawMargins.right() - tileWidth);

    QRectF rect = visibleArea().adjusted(-drawMargins.right(),
                                         -drawMargins.bottom(),
                                         drawMargins.left(),
                                         drawMargins.top());

    int startX = qMax((int) rect.x() / tileWidth, 0);
    int startY = qMax((int) rect.y() / tileHeight, 0);
    int endX = qMin((int) std::ceil(rect.right()) / tileWidth, layer->width() - 1);
    int endY = qMin((int) std::ceil(rect.bottom()) / tileHeight, layer->height() - 1);

    return QRect(QPoint(startX, startY), QPoint(endX, endY));
}

void MapItem::setHideCollisionLayer(bool hideCollisionLayer)
{
    if (mHideCollisionLayer == hideCollisionLayer)
        return;

    mHideCollisionLayer = hideCollisionLayer;
    refresh();

    emit hideCollisionLayerChanged();
}

QRectF MapItem::boundingRect() const
{
    if (!mRenderer)
        return QRectF();

    return QRectF(QPointF(), mRenderer->mapSize());
}

void MapItem::componentComplete()
{
    QQuickItem::componentComplete();
    if (mMapResource)
        refresh();
}

void MapItem::mapStatusChanged()
{
    if (mMapResource->status() == Resource::Ready)
        refresh();

    emit statusChanged();
}

void MapItem::refresh()
{
    if (!isComponentComplete())
        return;

    // Clean up ourselves (maybe wait until the map is available?)
    qDeleteAll(mTileLayerItems);
    mTileLayerItems.clear();

    qDeleteAll(mTileItems);
    mTileItems.clear();
    mFringeLayer = 0;
    mVisibleFringeTiles = QRect();

    delete mRenderer;
    mRenderer = 0;

    if (!mMapResource || mMapResource->status() != Resource::Ready)
        return;

    const Tiled::Map *map = mMapResource->map();

    switch (map->orientation()) {
    case Tiled::Map::Isometric:
        mRenderer = new Tiled::IsometricRenderer(map);
        break;
    default:
        mRenderer = new Tiled::OrthogonalRenderer(map);
        break;
    }

    foreach (Tiled::Layer *layer, map->layers()) {
        if (Tiled::TileLayer *tl = layer->asTileLayer()) {
            if (mHideCollisionLayer)
                if (tl->name().compare(QLatin1String("collision"), Qt::CaseInsensitive) == 0)
                    continue;

            if (!mFringeLayer) {
                if (tl->name().compare(QLatin1String("fringe"), Qt::CaseInsensitive) == 0) {
                    mFringeLayer = tl;
                    continue;
                }
            }

            TileLayerItem *layerItem = new TileLayerItem(tl, mRenderer, this);
            if (mFringeLayer)
                layerItem->setZ(65536);
            mTileLayerItems.append(layerItem);
        }
    }

    updateFringeLayer();

    const QSize size = mRenderer->mapSize();
    setImplicitSize(size.width(), size.height());
}

void MapItem::updateFringeLayer()
{
    if (!mFringeLayer)
        return;

    const QRect tileArea = visibleTileArea(mFringeLayer);
    if (mVisibleFringeTiles == tileArea)
        return;

    // Remove no longer visible fringe tiles
    for (int i = mTileItems.size() - 1; i >= 0; --i) {
        if (!tileArea.contains(mTileItems.at(i)->position())) {
            delete mTileItems.at(i);
            mTileItems.removeAt(i);
        }
    }

    // Create tile items for the newly visible area
    const QRegion n = QRegion(tileArea).subtracted(mVisibleFringeTiles);
    foreach (const QRect &r, n.rects()) {
        for (int y = r.top(); y <= r.bottom(); ++y) {
            for (int x = r.left(); x <= r.right(); ++x) {
                const Tiled::Cell &cell = mFringeLayer->cellAt(x, y);
                if (!cell.isEmpty())
                    mTileItems.append(new TileItem(cell, QPoint(x, y), this));
            }
        }
    }

    mVisibleFringeTiles = tileArea;
}
