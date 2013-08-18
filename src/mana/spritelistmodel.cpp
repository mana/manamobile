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

#include "spritelistmodel.h"

using namespace Mana;

SpriteListModel::SpriteListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    mRoleNames.insert(SpriteRole, "sprite");
    mRoleNames.insert(SlotRole, "slot");
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

QHash<int, QByteArray> SpriteListModel::roleNames() const
{
    return mRoleNames;
}

void SpriteListModel::addSprite(int slot, const SpriteReference *spriteRef)
{
    Q_ASSERT(spriteRef);
    beginInsertRows(QModelIndex(), mSprites.size(), mSprites.size());
    mSprites.append(QPair<int, const SpriteReference *>(slot, spriteRef));
    endInsertRows();
}

void SpriteListModel::setSprite(int slot, const SpriteReference *spriteRef)
{
    removeSprite(slot);
    addSprite(slot, spriteRef);
}

/**
 * Sets the list of sprites, assigning slots based on the index in the list.
 */
void SpriteListModel::setSprites(const QList<SpriteReference *> &sprites)
{
    beginResetModel();
    mSprites.clear();
    for (int i = 0; i < sprites.length(); ++i) {
        const SpriteReference *spriteRef = sprites.at(i);
        mSprites.append(qMakePair(i, spriteRef));
    }
    endResetModel();
}

void SpriteListModel::removeSprite(int slot)
{
    for (int i = 0, end = mSprites.size(); i < end; ++i) {
        if (mSprites.at(i).first == slot) {
            beginRemoveRows(QModelIndex(), i, i);
            mSprites.remove(i);
            endRemoveRows();
            return;
        }
    }
}

void SpriteListModel::removeAll()
{
    if (mSprites.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), 0, mSprites.size() - 1);
    mSprites.clear();
    endRemoveRows();
}
