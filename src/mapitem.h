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

#include <QDeclarativeItem>

class QNetworkReply;

namespace Tiled {
class Map;
class MapRenderer;
} // namespace Tiled

/**
 * A declarative item that displays a map.
 */
class MapItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    enum Status {
        Null,
        Ready,
        Error,
        Loading
    };

    explicit MapItem(QDeclarativeItem *parent = 0);

    QString source() const { return mSource; }
    void setSource(const QString &source);

    Status status() const { return mStatus; }

    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) {}

    void componentComplete();

signals:
    void sourceChanged(const QString &source);
    void statusChanged();

private slots:
    void mapFinished();
    void tilesetFinished();
    void imageFinished();

private:
    void setStatus(Status status);
    void load();
    QNetworkReply *finishReply();
    void checkReady();

    QString mSource;
    Status mStatus;

    Tiled::Map *mMap;
    Tiled::MapRenderer *mRenderer;

    QList<QNetworkReply*> mPendingResources;
};

#endif // MAPITEM_H
