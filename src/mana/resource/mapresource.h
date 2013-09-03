/*
 * Mana QML plugin
 * Copyright (C) 2010-2013  Thorbjørn Lindeijer
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

#ifndef MANA_MAPRESOURCE_H
#define MANA_MAPRESOURCE_H

#include "resource.h"

#include <QHash>
#include <QSet>

class QNetworkReply;

namespace Tiled {
class Map;
class TileLayer;
class Tileset;
}

namespace Mana {

class ImageResource;

class MapResource : public Resource
{
    Q_OBJECT

public:
    explicit MapResource(const QUrl &url,
                         const QString &path,
                         QObject *parent = 0);

    const Tiled::Map *map() const;
    const Tiled::TileLayer *collisionLayer() const;
    const ImageResource *tilesetImage(Tiled::Tileset *tileset) const;

private slots:
    void mapFinished();
    void tilesetFinished();
    void imageStatusChanged();

private:
    QNetworkReply *finishReply();
    void checkReady();
    void requestTilesetImage(Tiled::Tileset *tileset);

    QString mPath;
    Tiled::Map *mMap;
    Tiled::TileLayer *mCollisionLayer;

    QNetworkReply* mMapReply;
    QList<QNetworkReply*> mPendingResources;
    QSet<ImageResource*> mPendingImageResources;
    QHash<Tiled::Tileset*, ImageResource*> mImageResources;
};

inline const Tiled::Map *MapResource::map() const
{ return mMap; }

inline const Tiled::TileLayer *MapResource::collisionLayer() const
{ return mCollisionLayer; }

inline const ImageResource *MapResource::tilesetImage(Tiled::Tileset *tileset) const
{ return mImageResources.value(tileset); }

} // namespace Mana

Q_DECLARE_METATYPE(Mana::MapResource*)

#endif // MANA_MAPRESOURCE_H
