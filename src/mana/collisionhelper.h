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

#ifndef MANA_COLLISIONHELPER_H
#define MANA_COLLISIONHELPER_H

#include <QPointF>

namespace Tiled {
class TileLayer;
}

namespace Mana {

class CollisionHelper
{
public:
    CollisionHelper(const Tiled::TileLayer *collisionLayer)
        : mCollisionLayer(collisionLayer)
    {}

    QPointF adjustMove(QPointF pos, QPointF distance, qreal radius) const;

private:
    const Tiled::TileLayer *mCollisionLayer;
};

} // namespace Mana

#endif // MANA_COLLISIONHELPER_H
