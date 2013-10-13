/*
 * Mana Mobile
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

#include "shoplistmodel.h"

namespace Mana {

ShopListModel::ShopListModel(QObject *parent)
    : QAbstractListModel(parent)
    , mAmountRole(1, AmountRole)
{
    mRoleNames.insert(ItemIdRole, "itemId");
    mRoleNames.insert(AmountRole, "amount");
    mRoleNames.insert(CostRole, "cost");
}

int ShopListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mItems.size();
}

QVariant ShopListModel::data(const QModelIndex &index, int role) const
{
    const TradedItem &item = mItems.at(index.row());

    switch (role) {
    case ItemIdRole: return item.itemId;
    case AmountRole: return item.amount;
    case CostRole: return item.cost;
    }

    return QVariant();
}

QHash<int, QByteArray> ShopListModel::roleNames() const
{
    return mRoleNames;
}

void ShopListModel::setItems(const QVector<TradedItem> &items)
{
    beginResetModel();
    mItems = items;
    endResetModel();
}

void ShopListModel::removeItem(unsigned itemId, int amount)
{
    for (int i = 0; i < mItems.size(); ++i) {
        TradedItem &item = mItems[i];

        if (item.itemId == itemId) {
            item.amount -= amount;

            if (item.amount <= 0) {
                beginRemoveRows(QModelIndex(), i, i);
                mItems.remove(i);
                endRemoveRows();
            } else {
                const QModelIndex modelIndex = index(i);
                dataChanged(modelIndex, modelIndex, mAmountRole);
            }

            break;
        }
    }
}

} // namespace Mana
