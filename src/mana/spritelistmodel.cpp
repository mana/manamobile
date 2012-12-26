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

#include "spritelistmodel.h"

using namespace Mana;

SpriteListModel::SpriteListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(SpriteRole, "sprite");
    roleNames.insert(SlotRole, "slot");
    setRoleNames(roleNames);
}

int SpriteListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mSprites.size();
}

QVariant SpriteListModel::data(const QModelIndex &index, int role) const
{
    if (role == SpriteRole) {
        const SpriteReference *spriteRef = mSprites.at(index.row()).second;
        return QVariant::fromValue(spriteRef);
    } else if (role == SlotRole) {
        return QVariant::fromValue(mSprites.at(index.row()).first);
    }

    return QVariant();
}

void SpriteListModel::addSprite(int slot, const SpriteReference *spriteRef)
{
    Q_ASSERT(spriteRef);
    beginInsertRows(QModelIndex(), mSprites.size(), mSprites.size());
    mSprites.append(QPair<int, const SpriteReference *>(slot, spriteRef));
    endInsertRows();
}

void SpriteListModel::removeSprite(int slot)
{
    for (int i = 0, end = mSprites.size(); i < end; ++i) {
        if (mSprites.at(i).first == slot) {
            beginRemoveRows(QModelIndex(), i, i);
            mSprites.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

void SpriteListModel::removeAll()
{
    beginRemoveRows(QModelIndex(), 0, mSprites.size() - 1);
    mSprites.clear();
    endRemoveRows();
}
