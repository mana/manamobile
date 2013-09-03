/*
 * Mana QML plugin
 * Copyright (C) 2013  Thorbjørn Lindeijer
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

#include "collisionhelper.h"

#include "tiled/map.h"
#include "tiled/tilelayer.h"

#include <QtMath>

static bool freeSpace(const Tiled::TileLayer *collisionLayer,
                      int left, int top, int right, int bottom)
{
    if (left < 0 || top < 0)
        return false;
    if (right >= collisionLayer->width())
        return false;
    if (bottom >= collisionLayer->height())
        return false;

    for (int x = left; x <= right; ++x)
        for (int y = top; y <= bottom; ++y)
            if (!collisionLayer->cellAt(x, y).isEmpty())
                return false;

    return true;
}

namespace Mana {

/**
 * Adjusts the position \a pos by \a distance while taking into account
 * tile collisions and the collision \a radius.
 *
 * The distance is not allowed to be more than the tile size.
 */
QPointF CollisionHelper::adjustMove(QPointF pos, QPointF distance, qreal radius) const
{
    const Tiled::Map *map = mCollisionLayer->map();
    const int tw = map->tileWidth();
    const int th = map->tileHeight();

    const QRectF rect(pos.x() - radius,
                      pos.y() - radius,
                      radius * 2,
                      radius * 2);

    QPointF newPos = pos + distance;

    if (distance.x() > 0) {
        int currentTileX = qCeil(rect.right() / tw) - 1;
        int newTileX = qCeil((rect.right() + distance.x()) / tw) - 1;

        if (newTileX != currentTileX) {
            int minTileY = qFloor(rect.top() / th);
            int maxTileY = qCeil(rect.bottom() / th) - 1;

            if (!freeSpace(mCollisionLayer, newTileX, minTileY, newTileX, maxTileY))
                newPos.setX((currentTileX + 1) * tw - radius);
        }
    } else if (distance.x() < 0) {
        int currentTileX = qFloor(rect.left()) / tw;
        int newTileX = qFloor(rect.left() + distance.x()) / tw;

        if (currentTileX != newTileX) {
            int minTileY = qFloor(rect.top() / th);
            int maxTileY = qCeil(rect.bottom() / th) - 1;

            if (!freeSpace(mCollisionLayer, newTileX, minTileY, newTileX, maxTileY))
                newPos.setX(currentTileX * tw + radius);
        }
    }

    if (distance.y() > 0) {
        int currentTileY = qCeil(rect.bottom() / th) - 1;
        int newTileY = qCeil((rect.bottom() + distance.y()) / th) - 1;

        if (currentTileY != newTileY) {
            int minTileX = qFloor(rect.left() / tw);
            int maxTileX = qCeil(rect.right() / tw) - 1;

            if (!freeSpace(mCollisionLayer, minTileX, newTileY, maxTileX, newTileY))
                newPos.setY((currentTileY + 1) * th - radius);
        }
    } else if (distance.y() < 0) {
        int currentTileY = qFloor(qFloor(rect.top() / th));
        int newTileY = qFloor(rect.top() + distance.y()) / th;

        if (currentTileY != newTileY) {
            int minTileX = qFloor(rect.left() / tw);
            int maxTileX = qCeil(rect.right() / tw) - 1;

            if (!freeSpace(mCollisionLayer, minTileX, newTileY, maxTileX, newTileY))
                newPos.setY(currentTileY * th + radius);
        }
    }

    return newPos;
}

} // namespace Mana
