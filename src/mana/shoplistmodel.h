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

#ifndef MANA_SHOPLISTMODEL_H
#define MANA_SHOPLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

namespace Mana {

struct TradedItem
{
    unsigned itemId;
    int amount;
    int cost;
};

class ShopListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum AttributeRoles {
        ItemIdRole = Qt::UserRole,
        AmountRole,
        CostRole,
    };

    explicit ShopListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setItems(const QVector<TradedItem> &items);
    void removeItem(unsigned itemId, int amount);

public slots:

private:
    QHash<int, QByteArray> mRoleNames;
    QVector<int> mAmountRole;
    QVector<TradedItem> mItems;
};

} // namespace Mana

#endif // MANA_SHOPLISTMODEL_H
