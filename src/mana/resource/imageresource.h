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
    explicit ImageResource(const QString &filePath, QObject *parent = 0);

    ~ImageResource();

    QImage *image() { return mImage; }
    QSGTexture *texture(QQuickWindow *window) const;

private slots:
    void imageFinished();

private:
    QImage *mImage;
    mutable QSGTexture *mTexture;
};

}

#endif // IMAGERESOURCE_H
