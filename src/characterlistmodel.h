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

#ifndef CHARACTERLISTMODEL_H
#define CHARACTERLISTMODEL_H

#include <mana/accountclient.h>

#include <QAbstractListModel>

class CharacterListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum CharacterRoles {
        Name = Qt::DisplayRole,
        Level = Qt::UserRole,
        Money
    };

    explicit CharacterListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void setCharacters(const QList<Mana::CharacterInfo> &characters);

private:
    QList<Mana::CharacterInfo> mCharacters;
};

#endif // CHARACTERLISTMODEL_H
