/*
 * manalib
 * Copyright 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
 *
 * This file is part of manalib.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "imageset.h"

#include "resourcemanager.h"

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

QSGTexture *ImageSet::texture(QQuickWindow *window) const
{
    return mImage->texture(window);
}

QRect ImageSet::clip(int index) const
{
    const int framesPerRow = mImage->image()->width() / mWidth;
    int x = (index % framesPerRow) * mWidth;
    int y = (index / framesPerRow) * mHeight;
    return QRect(x, y, mWidth, mHeight);
}
