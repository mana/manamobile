/*
 * Mana QML plugin
 * Copyright (C) 2012  Erik Schilling 
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

#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "resource.h"

class QImage;
class QQuickWindow;
class QSGTexture;

namespace Mana {

class ImageResource : public Resource
{
    Q_OBJECT
public:
    explicit ImageResource(const QUrl &url, QObject *parent = 0);

    ~ImageResource();

    const QImage *image() const { return mImage; }
    QSGTexture *texture(const QQuickWindow *window) const;

private slots:
    void imageFinished();

private:
    QImage *mImage;
    mutable QSGTexture *mTexture;
};

}

#endif // IMAGERESOURCE_H
