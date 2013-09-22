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

#ifndef IMAGESET_H
#define IMAGESET_H

#include <QObject>

class QQuickWindow;
class QRect;
class QSGTexture;

namespace Mana {

class ImageResource;

class ImageSet : public QObject
{
    Q_OBJECT

public:
    explicit ImageSet(const QString &path,
                      int offsetX,
                      int offsetY,
                      int width,
                      int height,
                      QObject *parent = 0);
    ~ImageSet();

    int offsetX() const;
    int offsetY() const;

    const ImageResource *imageResource() const;

    QRectF clip(int index) const;

private:
    int mOffsetX;
    int mOffsetY;

    int mWidth;
    int mHeight;

    ImageResource *mImage;
};

inline int ImageSet::offsetX() const { return mOffsetX; }
inline int ImageSet::offsetY() const { return mOffsetY; }

inline const ImageResource *ImageSet::imageResource() const { return mImage; }

} // namespace Mana

#endif // IMAGESET_H
