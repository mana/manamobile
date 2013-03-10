/*
 * manalib
 * Copyright 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
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

#ifndef INVENTORYLISTMODEL_H
#define INVENTORYLISTMODEL_H

#include "messagein.h"
#include "protocol.h"

#include <QAbstractListModel>

namespace Mana {

class ItemInstance : public QObject
{
    Q_OBJECT

    Q_PROPERTY(unsigned slot READ slot CONSTANT)
    Q_PROPERTY(unsigned id READ id CONSTANT)
    Q_PROPERTY(unsigned amount READ amount NOTIFY amountChanged)
    Q_PROPERTY(unsigned equipmentSlot READ equipmentSlot NOTIFY equipmentChanged)
    Q_PROPERTY(bool isEquipped READ isEquipped NOTIFY equipmentChanged)

public:
    ItemInstance(unsigned slot, unsigned id,
                 unsigned amount, unsigned equipmentSlot,
                 QObject *parent);

    unsigned slot() const { return mSlot; }
    unsigned id() const { return mId; }

    unsigned amount() const { return mAmount; }
    void setAmount(unsigned newAmount);

    unsigned equipmentSlot() const { return mEquipmentSlot; }
    bool isEquipped() const { return mEquipmentSlot != 0; }
    void setEquipmentSlot(unsigned newEquipmentSlot);

signals:
    void amountChanged();
    void equipmentChanged();

private:
    unsigned mSlot;
    unsigned mId;
    unsigned mAmount;
    unsigned mEquipmentSlot;
};

class InventoryListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(ItemRoles)

public:
    enum ItemRoles {
        ItemRole = Qt::UserRole
    };

    explicit InventoryListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setItemSlot(unsigned slot, unsigned id,
                     unsigned amount, unsigned equipmentSlot);

    void equip(unsigned slot, unsigned equipmentSlot);
    void unequip(unsigned slot);

    void removeAllItems();

private:
    int indexBySlot(unsigned slot);
    void addItem(unsigned slot, unsigned id,
                 unsigned amount, unsigned equipmentSlot);
    void removeItem(int index);

    QList<ItemInstance *> mItems;

    QHash<int, QByteArray> mRoleNames;
};

}

Q_DECLARE_METATYPE(Mana::ItemInstance*)

#endif // INVENTORYLISTMODEL_H
