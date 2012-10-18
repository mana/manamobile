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

#ifndef PIXMAPRESOURCE_H
#define PIXMAPRESOURCE_H

#include "resource.h"

class QPixmap;

namespace Mana {

class PixmapResource : public Resource
{
    Q_OBJECT
public:
    explicit PixmapResource(const QString &filePath, QObject *parent = 0);

    ~PixmapResource();

    QPixmap *pixmap() { return mPixmap; }

private slots:
    void imageFinished();

private:
    QPixmap *mPixmap;

};

}

#endif // PIXMAPRESOURCE_H
