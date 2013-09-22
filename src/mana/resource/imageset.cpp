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

#include "imageset.h"

#include "mana/resource/imageresource.h"
#include "mana/resourcemanager.h"

#include <QImage>
#include <QRect>

using namespace Mana;

ImageSet::ImageSet(const QString &path,
                   int offsetX,
                   int offsetY,
                   int width,
                   int height,
                   QObject *parent)
    : QObject(parent)
    , mOffsetX(offsetX)
    , mOffsetY(offsetY)
    , mWidth(width)
    , mHeight(height)
{
    mImage = ResourceManager::instance()->requestImage(path);
}

ImageSet::~ImageSet()
{
    mImage->decRef();
}

QRectF ImageSet::clip(int index) const
{
    const int framesPerRow = mImage->image()->width() / mWidth;
    const int x = (index % framesPerRow) * mWidth;
    const int y = (index / framesPerRow) * mHeight;
    return QRectF(x, y, mWidth, mHeight);
}
