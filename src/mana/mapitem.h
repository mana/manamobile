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

#ifndef MAPITEM_H
#define MAPITEM_H

#include <QQuickItem>

class QNetworkReply;
class QSGTexture;

namespace Tiled {
class Map;
class MapRenderer;
class Tileset;
class TileLayer;
} // namespace Tiled

namespace Mana {

class ImageResource;
class MapResource;
class TileItem;
class TileLayerItem;

/**
 * A declarative item that displays a map.
 */
class MapItem : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(Mana::MapResource *mapResource READ mapResource WRITE setMapResource NOTIFY mapChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QRectF visibleArea READ visibleArea WRITE setVisibleArea NOTIFY visibleAreaChanged)
    Q_PROPERTY(bool hideCollisionLayer READ hideCollisionLayer WRITE setHideCollisionLayer NOTIFY hideCollisionLayerChanged)

public:
    enum Status {
        Null,
        Ready,
        Error,
        Loading
    };

    explicit MapItem(QQuickItem *parent = 0);

    MapResource *mapResource() const;
    void setMapResource(MapResource *mapResource);

    Status status() const;

    const QRectF &visibleArea() const;
    void setVisibleArea(const QRectF &visibleArea);
    QRect visibleTileArea(const Tiled::TileLayer *layer) const;

    bool hideCollisionLayer() const;
    void setHideCollisionLayer(bool hideCollisionLayer);

    QRectF boundingRect() const;

    void componentComplete();

signals:
    void mapChanged();
    void statusChanged();
    void visibleAreaChanged();
    void hideCollisionLayerChanged();

private slots:
    void mapStatusChanged();

private:
    void setStatus(Status status);
    void refresh();
    void updateFringeLayer();

    MapResource *mMapResource;
    QRectF mVisibleArea;
    QRect mVisibleFringeTiles;
    bool mHideCollisionLayer;

    Tiled::MapRenderer *mRenderer;
    Tiled::TileLayer *mFringeLayer;
    QList<TileLayerItem*> mTileLayerItems;
    QList<TileItem*> mTileItems;
};

inline const QRectF &MapItem::visibleArea() const
{ return mVisibleArea; }

inline bool MapItem::hideCollisionLayer() const
{ return mHideCollisionLayer; }

inline MapResource *MapItem::mapResource() const
{ return mMapResource; }

} // namespace Mana

#endif // MAPITEM_H
