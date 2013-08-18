/*
 * Mana Mobile
 * Copyright (C) 2010  Thorbjørn Lindeijer
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

#include "characterlistmodel.h"

#include "character.h"

using namespace Mana;

CharacterListModel::CharacterListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    mRoleNames.insert(CharRole, "character");
}

int CharacterListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mCharacters.size();
}

QVariant CharacterListModel::data(const QModelIndex &index, int role) const
{
    if (role == CharRole) {
        Character *charInfo = mCharacters.at(index.row());
        return QVariant::fromValue(charInfo);
    }

    return QVariant();
}

QHash<int, QByteArray> CharacterListModel::roleNames() const
{
    return mRoleNames;
}

void CharacterListModel::setCharacters(const QList<Character *> &
                                       characters)
{
    beginResetModel();
    mCharacters = characters;
    endResetModel();
}
