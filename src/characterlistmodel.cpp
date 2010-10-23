/*
 *  Mana Mobile
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *
 *  This file is part of Mana Mobile.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "characterlistmodel.h"

CharacterListModel::CharacterListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Name, "name");
    roleNames.insert(Level, "level");
    roleNames.insert(Money, "money");
    setRoleNames(roleNames);
}

int CharacterListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mCharacters.size();
}

QVariant CharacterListModel::data(const QModelIndex &index, int role) const
{
    const Mana::CharacterInfo &character = mCharacters.at(index.row());

    switch (role) {
    case Name:
        return character.name;
    case Level:
        return character.level;
    case Money:
        return character.money;
    }

    return QVariant();
}

void CharacterListModel::setCharacters(const QList<Mana::CharacterInfo> &
                                       characters)
{
    beginResetModel();
    mCharacters = characters;
    endResetModel();
}
