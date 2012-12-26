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

#ifndef SPRITELISTMODEL_H
#define SPRITELISTMODEL_H

#include <QAbstractListModel>

#include "resource/spritedef.h"

namespace Mana {

class SpriteReference;

class SpriteListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum SpriteRoles {
        SpriteRole = Qt::UserRole,
        SlotRole
    };

    explicit SpriteListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void addSprite(int slot, const SpriteReference *spriteRef);
    void removeSprite(int slot);
    void removeAll();

private:
    QList< QPair<int, const SpriteReference *> > mSprites;

};

}

#endif // SPRITELISTMODEL_H
