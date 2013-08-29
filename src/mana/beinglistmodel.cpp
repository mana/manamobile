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

#include "beinglistmodel.h"

#include "being.h"

#include <safeassert.h>

using namespace Mana;

BeingListModel::BeingListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mRoleNames.insert(BeingRole, "being");
}

int BeingListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mBeings.size();
}

QVariant BeingListModel::data(const QModelIndex &index, int role) const
{
    if (role == BeingRole) {
        Being *being = mBeings.at(index.row());
        return QVariant::fromValue(being);
    }

    return QVariant();
}

QHash<int, QByteArray> BeingListModel::roleNames() const
{
    return mRoleNames;
}

void BeingListModel::clear()
{
    if (mBeings.isEmpty())
        return;

    // Remove all beings from the model
    beginRemoveRows(QModelIndex(), 0, mBeings.size() - 1);
    qDeleteAll(mBeings);
    mBeings.clear();
    endRemoveRows();
}

Being *BeingListModel::beingById(int id) const
{
    int index = indexOfBeing(id);
    if (index != -1)
        return beingAt(index);
    return 0;
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
    SAFE_ASSERT(index != -1, return);

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
