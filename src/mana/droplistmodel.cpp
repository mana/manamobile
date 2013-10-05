/*
 * Mana QML plugin
 * Copyright (C) 2013  Erik Schilling
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

#include "droplistmodel.h"

using namespace Mana;

DropListModel::DropListModel(QObject *parent):
    QAbstractListModel(parent)
{
    mRoleNames.insert(DropRole, "drop");
}

int DropListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mDropList.size();
}

QVariant DropListModel::data(const QModelIndex &index, int role) const
{
    if (role == DropRole) {
        Drop *drop = mDropList.at(index.row());
        return QVariant::fromValue(drop);
    }

    return QVariant();
}

QHash<int, QByteArray> DropListModel::roleNames() const
{
    return mRoleNames;
}

void DropListModel::addDrop(unsigned id, const QPoint &position)
{
    beginInsertRows(QModelIndex(), mDropList.size(),
                    mDropList.size());
    mDropList.append(new Drop(id, position, this));
    endInsertRows();
}

void DropListModel::removeDrop(const QPoint &position)
{
    QList<Drop *>::iterator it, it_end;
    for (it = mDropList.begin(), it_end = mDropList.end(); it != it_end; ++it) {
        if ((*it)->position() == position)
            break;
    }

    if (it == it_end)
        return;

    int index = it - mDropList.begin();
    beginRemoveRows(QModelIndex(), index, index);
    mDropList.removeAt(index);
    endRemoveRows();
}

void DropListModel::clear()
{
    beginResetModel();
    mDropList.clear();
    endResetModel();
}

