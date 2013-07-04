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

#ifndef CHARACTERLISTMODEL_H
#define CHARACTERLISTMODEL_H

#include <QAbstractListModel>

namespace Mana {

class Character;

class CharacterListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum CharacterRoles {
        CharRole = Qt::UserRole
    };

    explicit CharacterListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setCharacters(const QList<Character *> &characters);

private:
    QList<Character *> mCharacters;
    QHash<int, QByteArray> mRoleNames;
};

} // namespace Mana

#endif // CHARACTERLISTMODEL_H
