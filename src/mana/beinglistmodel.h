/*
 * Mana QML plugin
 * Copyright (C) 2010  Thorbjørn Lindeijer 
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

#ifndef BEINGLISTMODEL_H
#define BEINGLISTMODEL_H

#include <QAbstractListModel>

namespace Mana {

class Being;
class Character;
class MessageIn;

class BeingListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(BeingRoles)

public:
    enum BeingRoles {
        BeingRole = Qt::UserRole
    };

    explicit BeingListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE Mana::Being *closestBeingAround(Mana::Being *center) const;

    Being *beingById(int id) const;
    void addBeing(Being *being);
    void removeBeing(int id);
    const QList<Being*> &beings() const { return mBeings; }

    void clear();

private:
    Being *beingAt(int index) const { return mBeings.at(index); }
    int indexOfBeing(int id) const;

    QList<Being*> mBeings;
    QHash<int, QByteArray> mRoleNames;
};

} // namespace Mana

#endif // BEINGLISTMODEL
