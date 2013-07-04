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

#ifndef TILELAYERITEM_H
#define TILELAYERITEM_H

#include <QQuickItem>

namespace Tiled {
class TileLayer;
class MapRenderer;
}

namespace Mana {

class MapItem;

/**
 * A graphics item displaying a tile layer in a QGraphicsView.
 */
class TileLayerItem : public QQuickItem
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param layer    the tile layer to be displayed
     * @param renderer the map renderer to use to render the layer
     */
    TileLayerItem(Tiled::TileLayer *layer, Tiled::MapRenderer *renderer,
                  MapItem *parent);

    /**
     * Updates the size and position of this item. Should be called when the
     * size of either the tile layer or its associated map have changed.
     *
     * Calling this function when the size of the map changes is necessary
     * because in certain map orientations this affects the layer position
     * (when using the IsometricRenderer for example).
     */
    void syncWithTileLayer();

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *);

public slots:
    void updateVisibleTiles();

private:
    Tiled::TileLayer *mLayer;
    Tiled::MapRenderer *mRenderer;
    QRect mVisibleTiles;
};

} // namespace Mana

#endif // TILELAYERITEM_H
