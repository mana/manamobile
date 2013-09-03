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
    const int tileWidth = map->tileWidth();
    const int tileHeight = map->tileHeight();
    bool blocked = false;
    bool reachedNewTileX = false;
    bool reachedNewTileY = false;

    const QRectF rect(pos.x() - radius,
                      pos.y() - radius,
                      radius * 2,
                      radius * 2);

    QPointF newPos = pos + distance;

    if (distance.x() > 0) {
        const int currentTileX = qCeil(rect.right() / tileWidth) - 1;
        const int newTileX = qCeil((rect.right() + distance.x()) / tileWidth) - 1;

        if (newTileX != currentTileX) {
            reachedNewTileX = true;
            const int minTileY = qFloor(rect.top() / tileHeight);
            const int maxTileY = qCeil(rect.bottom() / tileHeight) - 1;

            if (!freeSpace(mCollisionLayer, newTileX, minTileY, newTileX, maxTileY)) {
                newPos.setX((currentTileX + 1) * tileWidth - radius);
                blocked = true;
            }
        }
    } else if (distance.x() < 0) {
        const int currentTileX = qFloor(rect.left()) / tileWidth;
        const int newTileX = qFloor(rect.left() + distance.x()) / tileWidth;

        if (currentTileX != newTileX) {
            reachedNewTileX = true;
            const int minTileY = qFloor(rect.top() / tileHeight);
            const int maxTileY = qCeil(rect.bottom() / tileHeight) - 1;

            if (!freeSpace(mCollisionLayer, newTileX, minTileY, newTileX, maxTileY)) {
                newPos.setX(currentTileX * tileWidth + radius);
                blocked = true;
            }
        }
    }

    if (distance.y() > 0) {
        const int currentTileY = qCeil(rect.bottom() / tileHeight) - 1;
        const int newTileY = qCeil((rect.bottom() + distance.y()) / tileHeight) - 1;

        if (currentTileY != newTileY) {
            reachedNewTileY = true;
            const int minTileX = qFloor(rect.left() / tileWidth);
            const int maxTileX = qCeil(rect.right() / tileWidth) - 1;

            if (!freeSpace(mCollisionLayer, minTileX, newTileY, maxTileX, newTileY)) {
                newPos.setY((currentTileY + 1) * tileHeight - radius);
                blocked = true;
            }
        }
    } else if (distance.y() < 0) {
        const int currentTileY = qFloor(qFloor(rect.top() / tileHeight));
        const int newTileY = qFloor(rect.top() + distance.y()) / tileHeight;

        if (currentTileY != newTileY) {
            reachedNewTileY = true;
            const int minTileX = qFloor(rect.left() / tileWidth);
            const int maxTileX = qCeil(rect.right() / tileWidth) - 1;

            if (!freeSpace(mCollisionLayer, minTileX, newTileY, maxTileX, newTileY)) {
                newPos.setY(currentTileY * tileHeight + radius);
                blocked = true;
            }
        }
    }

    // This is a little unclean, but prevents walking into a tile by its corner.
    if (reachedNewTileX && reachedNewTileY && !blocked) {
        const QRectF newRect = rect.translated(distance);
        const int left = qFloor(newRect.left() / tileWidth);
        const int top = qFloor(newRect.top() / tileHeight);
        const int right = qCeil(newRect.right() / tileWidth) - 1;
        const int bottom = qCeil(newRect.bottom() / tileHeight) - 1;

        if (!freeSpace(mCollisionLayer, left, top, right, bottom))
            newPos.setX(pos.x());
    }

    return newPos;
}

} // namespace Mana
