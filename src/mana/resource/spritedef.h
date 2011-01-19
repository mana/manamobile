/*
 * manalib
 * Copyright 2011, Jared Adams <jaxad0127@gmail.com>
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

#ifndef SPRITEDEF_H
#define SPRITEDEF_H

#include <QList>
#include <QMap>
#include <QPixmap>
#include <QString>

#include "../being.h"
#include "../protocol.h"

class XmlReader;

class QNetworkReply;

namespace Mana {

class SpriteReference : public QObject
{
public:
    static SpriteReference *Empty;

    enum Gender {
        UNKNOWN, MALE, FEMALE
    };

    Q_ENUMS(Gender)

    SpriteReference(QObject *parent, QString sprite = "", int variant = 0);

    QString sprite;
    int variant;
};

struct SpriteDisplay
{
    QString image;
    QList<SpriteReference*> sprites;
    QList<QString> particles;
};

} // namespace Mana

#endif // SPRITEDEF_H
