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

#include "tilelayeritem.h"

#include "tiled/tile.h"
#include "tiled/tilelayer.h"
#include "tiled/map.h"
#include "tiled/maprenderer.h"

#include <QPainter>

using namespace Tiled;
using namespace Mana;

TileLayerItem::TileLayerItem(TileLayer *layer, MapRenderer *renderer,
                             QQuickItem *parent)
    : QQuickItem(parent)
    , mLayer(layer)
    , mRenderer(renderer)
{
    setFlag(ItemHasContents);

    syncWithTileLayer();
    setOpacity(mLayer->opacity());
}

void TileLayerItem::syncWithTileLayer()
{
    const QRectF boundingRect = mRenderer->boundingRect(mLayer->bounds());
    setPosition(boundingRect.topLeft());
    setSize(boundingRect.size());
}

QSGNode *TileLayerItem::updatePaintNode(QSGNode *,
                                        QQuickItem::UpdatePaintNodeData *)
{
    // TODO
}
