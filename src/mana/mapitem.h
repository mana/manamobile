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

#ifndef MAPITEM_H
#define MAPITEM_H

#include <QHash>
#include <QQuickItem>

class QNetworkReply;
class QSGTexture;

namespace Tiled {
class Map;
class MapRenderer;
class Tileset;
} // namespace Tiled

namespace Mana {

class ImageResource;
class TileLayerItem;

/**
 * A declarative item that displays a map.
 */
class MapItem : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QRectF visibleArea READ visibleArea WRITE setVisibleArea NOTIFY visibleAreaChanged)

public:
    enum Status {
        Null,
        Ready,
        Error,
        Loading
    };

    explicit MapItem(QQuickItem *parent = 0);

    QString source() const { return mSource; }
    void setSource(const QString &source);

    Status status() const { return mStatus; }

    const QRectF &visibleArea() const;
    void setVisibleArea(const QRectF &visibleArea);

    QRectF boundingRect() const;

    void componentComplete();

    const Tiled::Map *map() const;
    const ImageResource *tilesetImage(Tiled::Tileset *tileset) const;

signals:
    void sourceChanged(const QString &source);
    void statusChanged();
    void visibleAreaChanged();

private slots:
    void mapFinished();
    void tilesetFinished();
    void imageStatusChanged();

private:
    void setStatus(Status status);
    void load();
    QNetworkReply *finishReply();
    void checkReady();
    void requestTilesetImage(Tiled::Tileset *tileset);

    QString mSource;
    Status mStatus;
    QRectF mVisibleArea;

    Tiled::Map *mMap;
    Tiled::MapRenderer *mRenderer;

    QNetworkReply* mMapReply;
    QList<QNetworkReply*> mPendingResources;
    QSet<ImageResource*> mPendingImageResources;
    QHash<Tiled::Tileset*, ImageResource*> mImageResources;
    QList<TileLayerItem*> mTileLayerItems;
};

inline const QRectF &MapItem::visibleArea() const
{ return mVisibleArea; }

inline const Tiled::Map *MapItem::map() const
{ return mMap; }

inline const ImageResource *MapItem::tilesetImage(Tiled::Tileset *tileset) const
{ return mImageResources.value(tileset); }

} // namespace Mana

#endif // MAPITEM_H
