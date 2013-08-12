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
        // Taking into account the normalized texture subrectancle
        const float s_width = data.width * s_x;
        const float s_height = data.height * s_y;
        const float s_tx = r.x() + data.tx * s_x;
        const float s_ty = r.y() + data.ty * s_y;

        // TopLeft                      // TopRight
        v[0].x = data.x;                v[2].x = data.x + data.width;
        v[0].y = data.y;                v[2].y = data.y;
        v[0].tx = s_tx;                 v[2].tx = s_tx + s_width;
        v[0].ty = s_ty;                 v[2].ty = s_ty;

        // BottomLeft
        v[1].x = data.x;
        v[1].y = data.y + data.height;
        v[1].tx = s_tx;
        v[1].ty = s_ty + s_height;


                                        // TopRight
                                        v[5].x = data.x + data.width;
                                        v[5].y = data.y;
                                        v[5].tx = s_tx + s_width;
                                        v[5].ty = s_ty;

        // BottomLeft                   // BottomRight
        v[3].x = data.x;                v[4].x = data.x + data.width;
        v[3].y = data.y + data.height;  v[4].y = data.y + data.height;
        v[3].tx = s_tx;                 v[4].tx = s_tx + s_width;
        v[3].ty = s_ty + s_height;      v[4].ty = s_ty + s_height;

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
 * This helper class exists mainly to avoid redoing calculations that only need
 * to be done once per tileset.
 */
struct TilesetHelper
{
    TilesetHelper(const MapItem *mapItem)
        : mMapItem(mapItem)
        , mWindow(mapItem->window())
        , mTileset(0)
        , mTexture(0)
        , mMargin(0)
        , mTileHSpace(0)
        , mTileVSpace(0)
        , mTilesPerRow(0)
    {
    }

    Tileset *tileset() const { return mTileset; }
    QSGTexture *texture() const { return mTexture; }

    void setTileset(Tileset *tileset)
    {
        mTileset = tileset;
        mTexture = tilesetTexture(tileset, mMapItem, mWindow);
        if (!mTexture)
            return;

        const int tileSpacing = tileset->tileSpacing();
        mMargin = tileset->margin();
        mTileHSpace = tileset->tileWidth() + tileSpacing;
        mTileVSpace = tileset->tileHeight() + tileSpacing;

        const QSize tilesetSize = mTexture->textureSize();
        const int availableWidth = tilesetSize.width() + tileSpacing - mMargin;
        mTilesPerRow = availableWidth / mTileHSpace;
    }

    void setTextureCoordinates(TileData &data, const Cell &cell) const
    {
        const int tileId = cell.tile->id();
        const int column = tileId % mTilesPerRow;
        const int row = tileId / mTilesPerRow;

        data.tx = column * mTileHSpace + mMargin;
        data.ty = row * mTileVSpace + mMargin;
    }

private:
    const MapItem *mMapItem;
    QQuickWindow *mWindow;
    Tileset *mTileset;
    QSGTexture *mTexture;
    int mMargin;
    int mTileHSpace;
    int mTileVSpace;
    int mTilesPerRow;
};

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
    TilesetHelper helper(mapItem);

    const Map *map = mapItem->map();
    const int tileWidth = map->tileWidth();
    const int tileHeight = map->tileHeight();

    QVector<TileData> tileData;

    for (int y = rect.top(); y < rect.bottom(); ++y) {
        for (int x = rect.left(); x < rect.right(); ++x) {
            const Cell &cell = layer->cellAt(x, y);
            if (cell.isEmpty())
                continue;

            Tileset *tileset = cell.tile->tileset();

            if (tileset != helper.tileset()) {
                if (!tileData.isEmpty()) {
                    parent->appendChildNode(new TilesNode(helper.texture(),
                                                          tileData));
                    tileData.clear();
                }

                helper.setTileset(tileset);
            }

            if (!helper.texture())
                continue;

            const QSize size = cell.tile->size();
            const QPoint offset = tileset->tileOffset();

            TileData data;
            data.x = x * tileWidth + offset.x();
            data.y = (y + 1) * tileHeight - tileset->tileHeight() + offset.y();
            data.width = size.width();
            data.height = size.height();
            helper.setTextureCoordinates(data, cell);
            tileData.append(data);
        }
    }

    if (!tileData.isEmpty())
        parent->appendChildNode(new TilesNode(helper.texture(), tileData));
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
