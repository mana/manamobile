/*
 * Mana Mobile
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

#ifndef MANA_TILESNODE_H
#define MANA_TILESNODE_H

#include <QSGGeometryNode>
#include <QSGTextureMaterial>

namespace Mana {

struct TileData {
    float x;
    float y;
    float width;
    float height;
    float tx;
    float ty;
};

class TilesNode : public QSGGeometryNode
{
public:
    TilesNode(QSGTexture *texture, const QVector<TileData> &tileData);

    QSGTexture *texture() const;

private:
    void processTileData(const QVector<TileData> &tileData);

    QSGGeometry mGeometry;
    QSGTextureMaterial mMaterial;
    QSGOpaqueTextureMaterial mOpaqueMaterial;
};

inline QSGTexture *TilesNode::texture() const
{
    return mMaterial.texture();
}

} // namespace Mana

#endif // MANA_TILESNODE_H
