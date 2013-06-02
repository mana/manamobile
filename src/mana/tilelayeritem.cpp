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

#include "mana/mapitem.h"
#include "mana/resource/imageresource.h"

#include <QSGGeometryNode>
#include <QSGTextureMaterial>

#include <cmath>

using namespace Tiled;
using namespace Mana;

namespace {

struct TileData {
    Cell cell;
    float x;
    float y;
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

TilesNode::TilesNode(QSGTexture *texture, const QVector<TileData> &tileData)
    : mGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0)
{
    setFlag(QSGNode::OwnedByParent);

    mMaterial.setTexture(texture);
    mOpaqueMaterial.setTexture(texture);

    mGeometry.setDrawingMode(GL_TRIANGLES);

    // TODO: Using StaticPattern would make sense since the data is never
    // being modified, however this is causing issues on the Raspberry Pi.
    //mGeometry.setVertexDataPattern(QSGGeometry::StaticPattern);

    processTileData(tileData);

    setGeometry(&mGeometry);
    setMaterial(&mMaterial);
    setOpaqueMaterial(&mOpaqueMaterial);
}

inline QSGTexture *TilesNode::texture() const
{
    return mMaterial.texture();
}

void TilesNode::processTileData(const QVector<TileData> &tileData)
{
    const QSize s = mMaterial.texture()->textureSize();
    const QRectF r = mMaterial.texture()->normalizedTextureSubRect();

    const float s_x = r.width() / s.width();
    const float s_y = r.height() / s.height();

    // TODO: By using indices the memory usage could be reduced by 25%, at the
    // cost of an additional indirection:
    //
    // Currently each tile takes:   6 * 16         = 96 bytes
    // With indices it would take:  4 * 16 + 4 * 2 = 72 bytes

    // Two triangles to draw each tile
    mGeometry.allocate(tileData.size() * 6);
    QSGGeometry::TexturedPoint2D *v = mGeometry.vertexDataAsTexturedPoint2D();

    foreach (const TileData &data, tileData) {
        const QSize size = data.cell.tile->size();
        const float width = size.width();
        const float height = size.height();

        // Taking into account the normalized texture subrectancle
        const float s_width = width * s_x;
        const float s_height = height * s_y;
        const float s_tx = r.x() + data.tx * s_x;
        const float s_ty = r.y() + data.ty * s_y;

        // TopLeft                  // TopRight
        v[0].x = data.x;            v[2].x = data.x + width;
        v[0].y = data.y;            v[2].y = data.y;
        v[0].tx = s_tx;             v[2].tx = s_tx + s_width;
        v[0].ty = s_ty;             v[2].ty = s_ty;

        // BottomLeft
        v[1].x = data.x;
        v[1].y = data.y + height;
        v[1].tx = s_tx;
        v[1].ty = s_ty + s_height;


                                    // TopRight
                                    v[5].x = data.x + width;
                                    v[5].y = data.y;
                                    v[5].tx = s_tx + s_width;
                                    v[5].ty = s_ty;

        // BottomLeft               // BottomRight
        v[3].x = data.x;            v[4].x = data.x + width;
        v[3].y = data.y + height;   v[4].y = data.y + height;
        v[3].tx = s_tx;             v[4].tx = s_tx + s_width;
        v[3].ty = s_ty + s_height;  v[4].ty = s_ty + s_height;

        v += 6;
    }

    markDirty(DirtyGeometry);
}

/**
 * Determines the rectangle of visible tiles of the given tile \a layer, based
 * on the visible area of a \a mapItem.
 *
 * Only works for orthogonal maps.
 */
static QRect visibleTiles(const TileLayer *layer,
                          const MapItem *mapItem)
{
    const Map *map = mapItem->map();

    const int tileWidth = map->tileWidth();
    const int tileHeight = map->tileHeight();

    QMargins drawMargins = layer->drawMargins();
    drawMargins.setTop(drawMargins.top() - tileHeight);
    drawMargins.setRight(drawMargins.right() - tileWidth);

    QRectF rect = mapItem->visibleArea().adjusted(-drawMargins.right(),
                                                  -drawMargins.bottom(),
                                                  drawMargins.left(),
                                                  drawMargins.top());

    int startX = qMax((int) rect.x() / tileWidth, 0);
    int startY = qMax((int) rect.y() / tileHeight, 0);
    int endX = qMin((int) std::ceil(rect.right()) / tileWidth + 1, layer->width());
    int endY = qMin((int) std::ceil(rect.bottom()) / tileHeight + 1, layer->height());

    return QRect(QPoint(startX, startY), QPoint(endX, endY));
}

/**
 * Returns the texture of a given tileset, or 0 if the image has not been
 * loaded yet.
 */
static inline QSGTexture *tilesetTexture(Tileset *tileset,
                                         const MapItem *mapItem,
                                         QQuickWindow *window)
{
    if (const ImageResource *image = mapItem->tilesetImage(tileset))
        if (image->isReady())
            return image->texture(window);
    return 0;
}

/**
 * Draws an orthogonal tile layer by adding nodes to the scene graph. As long
 * sequentially drawn tiles are using the same tileset, they will share a
 * single geometry node.
 */
static void drawTileLayer(QSGNode *parent,
                          const MapItem *mapItem,
                          const TileLayer *layer,
                          const QRect &rect)
{
    QQuickWindow *window = mapItem->window();

    const Map *map = mapItem->map();
    const int tileWidth = map->tileWidth();
    const int tileHeight = map->tileHeight();

    QSGTexture *currentTexture = 0;
    int currentTextureId = 0;
    QVector<TileData> tileData;

    for (int y = rect.top(); y < rect.bottom(); ++y) {
        for (int x = rect.left(); x < rect.right(); ++x) {
            const Cell &cell = layer->cellAt(x, y);
            if (cell.isEmpty())
                continue;

            Tileset *tileset = cell.tile->tileset();
            QSGTexture *texture = tilesetTexture(tileset, mapItem, window);

            if (texture) {
                const int textureId = texture->textureId();

                if (textureId != currentTextureId) {
                    if (!tileData.isEmpty()) {
                        parent->appendChildNode(new TilesNode(currentTexture,
                                                              tileData));
                        tileData.clear();
                    }

                    currentTexture = texture;
                    currentTextureId = textureId;
                }

                const QSize tilesetSize = texture->textureSize();
                const int tilesPerRow = tilesetSize.width() / tileset->tileWidth();

                const int tileId = cell.tile->id();
                const int column = tileId % tilesPerRow;
                const int row = tileId / tilesPerRow;

                TileData data;
                data.cell = cell;
                data.x = x * tileWidth;
                data.y = (y + 1) * tileHeight - tileset->tileHeight();
                data.tx = column * tileset->tileWidth();
                data.ty = row * tileset->tileHeight();
                tileData.append(data);
            }
        }
    }

    if (!tileData.isEmpty())
        parent->appendChildNode(new TilesNode(currentTexture, tileData));
}

} // anonymous namespace


TileLayerItem::TileLayerItem(TileLayer *layer, MapRenderer *renderer,
                             MapItem *parent)
    : QQuickItem(parent)
    , mLayer(layer)
    , mRenderer(renderer)
    , mVisibleTiles(visibleTiles(layer, parent))
{
    setFlag(ItemHasContents);

    connect(parent, SIGNAL(visibleAreaChanged()), SLOT(updateVisibleTiles()));

    syncWithTileLayer();
    setOpacity(mLayer->opacity());
}

void TileLayerItem::syncWithTileLayer()
{
    const QRectF boundingRect = mRenderer->boundingRect(mLayer->bounds());
    setPosition(boundingRect.topLeft());
    setSize(boundingRect.size());
}



QSGNode *TileLayerItem::updatePaintNode(QSGNode *node,
                                        QQuickItem::UpdatePaintNodeData *)
{
    const MapItem *mapItem = static_cast<MapItem*>(parentItem());

    if (!node) {
        node = new QSGNode;
        node->setFlag(QSGNode::OwnedByParent);
    }

    node->removeAllChildNodes();
    drawTileLayer(node, mapItem, mLayer, mVisibleTiles);

    return node;
}

void TileLayerItem::updateVisibleTiles()
{
    const MapItem *mapItem = static_cast<MapItem*>(parentItem());
    const QRect rect = visibleTiles(mLayer, mapItem);

    if (mVisibleTiles != rect) {
        mVisibleTiles = rect;
        update();
    }
}
