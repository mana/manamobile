/*
 * resourcelistmodel.cpp
 * Copyright (C) 2013  Thorbjørn Lindeijer 
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

#include "resourcelistmodel.h"

#include <cassert>

namespace Mana {

ResourceListModel::ResourceListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int ResourceListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mResources.size();
}

QVariant ResourceListModel::data(const QModelIndex &index, int role) const
{
    if (role == ResourceRole)
        return QVariant::fromValue(mResources.at(index.row()));

    return QVariant();
}

QHash<int, QByteArray> ResourceListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ResourceRole, "resource");
    return roles;
}

void ResourceListModel::addResource(Resource *resource)
{
    beginInsertRows(QModelIndex(), mResources.size(), mResources.size());
    mResources.append(resource);
    endInsertRows();
}

void ResourceListModel::removeResource(Resource *resource)
{
    const int index = mResources.indexOf(resource);
    assert(index != -1);

    beginRemoveRows(QModelIndex(), index, index);
    mResources.removeAt(index);
    endRemoveRows();
}

} // namespace Mana
