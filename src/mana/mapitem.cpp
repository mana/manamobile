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

using namespace Mana;

MapItem::MapItem(QQuickItem *parent)
    : QQuickItem(parent)
    , mMapResource(0)
    , mRenderer(0)
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

    bool seenFringe = false;
    foreach (Tiled::Layer *layer, map->layers()) {
        if (Tiled::TileLayer *tl = layer->asTileLayer()) {
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
}
