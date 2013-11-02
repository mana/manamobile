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
    QHash<int, QByteArray> roleNames() const;

    void addSprite(int slot, SpriteReference *spriteRef);
    void setSprite(int slot, SpriteReference *spriteRef);
    void setSprites(const QList<SpriteReference*> &sprites);
    void removeSprite(int slot);
    void removeAll();

private:
    QVector< QPair<int, SpriteReference *> > mSprites;
    QHash<int, QByteArray> mRoleNames;
};

}

#endif // SPRITELISTMODEL_H
