/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#include "beinglistmodel.h"

#include <QDebug>

using namespace Mana;

BeingListModel::BeingListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(BeingName, "name");
    roleNames.insert(BeingX, "x");
    roleNames.insert(BeingY, "y");
    setRoleNames(roleNames);
}

int BeingListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mBeings.size();
}

QVariant BeingListModel::data(const QModelIndex &index, int role) const
{
    const Being *being = mBeings.at(index.row());

    switch (role) {
    case BeingName:
        return being->name();
    case BeingX:
        return being->x();
    case BeingY:
        return being->y();
    default:
        return QVariant();
    }
}

void BeingListModel::addBeing(Being *being)
{
    beginInsertRows(QModelIndex(), mBeings.size(), mBeings.size());
    mBeings.append(being);
    endInsertRows();
}

void BeingListModel::removeBeing(int id)
{
    const int index = indexOfBeing(id);
    beginRemoveRows(QModelIndex(), index, index);
    delete mBeings.takeAt(index);
    endRemoveRows();
}

int BeingListModel::indexOfBeing(int id) const
{
    // TODO: Would be better to have a map/hash
    for (int i = 0, end = mBeings.size(); i < end; i++) {
        Being *being = mBeings.at(i);
        if (being->id() == id)
            return i;
    }
    return -1;
}

void BeingListModel::notifyBeingChanged(int index)
{
    const QModelIndex modelIndex = QAbstractListModel::index(index);
    emit dataChanged(modelIndex, modelIndex);
}
