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

#include "inventorylistmodel.h"

using namespace Mana;

ItemInstance::ItemInstance(unsigned slot, unsigned id,
                           unsigned amount, unsigned equipmentSlot,
                           QObject *parent)
    : QObject(parent)
    , mSlot(slot)
    , mId(id)
    , mAmount(amount)
    , mEquipmentSlot(equipmentSlot)
{
}

void ItemInstance::setAmount(unsigned newAmount)
{
    if (mAmount != newAmount) {
        mAmount = newAmount;
        emit amountChanged();
    }
}

void ItemInstance::setEquipmentSlot(unsigned newSlot)
{
    if (mEquipmentSlot != newSlot) {
        mEquipmentSlot = newSlot;
        emit equipmentChanged();
    }
}

InventoryListModel::InventoryListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mRoleNames.insert(ItemRole, "item");
#if QT_VERSION < 0x050000
    setRoleNames(mRoleNames);
#endif
}

int InventoryListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mItems.size();
}

QVariant InventoryListModel::data(const QModelIndex &index, int role) const
{
    if (role == ItemRole) {
        ItemInstance *item = mItems.at(index.row());
        return QVariant::fromValue(item);
    }

    return QVariant();
}

QHash<int, QByteArray> InventoryListModel::roleNames() const
{
    return mRoleNames;
}

void InventoryListModel::handleInventory(MessageIn &message)
{
    while (message.unreadLength())
    {
        unsigned slot = message.readInt16();
        int id = message.readInt16(); // 0 id means removal of slot
        unsigned amount = id ? message.readInt16() : 0;
        unsigned equipmentSlot = id ? message.readInt16() : 0;

        setItemSlot(slot, id, amount, equipmentSlot);
    }
}

void InventoryListModel::handleInventoryFull(MessageIn &message)
{
    removeAllItems();

    int count = message.readInt16();
    while (count--)
    {
        int slot = message.readInt16();
        int id = message.readInt16();
        int amount = message.readInt16();
        int equipmentSlot = message.readInt16();
        setItemSlot(slot, id, amount, equipmentSlot);
    }
}

void InventoryListModel::handleEquip(MessageIn &message)
{
    const unsigned slot = message.readInt16();
    int index = indexBySlot(slot);

    if (index == -1)
        return;

    ItemInstance *item = mItems.at(index);
    item->setEquipmentSlot(message.readInt16());
}

void InventoryListModel::handleUnEquip(MessageIn &message)
{
    const unsigned slot = message.readInt16();
    int index = indexBySlot(slot);

    if (index == -1)
        return;

    ItemInstance *item = mItems.at(index);
    item->setEquipmentSlot(0);
}

int InventoryListModel::indexBySlot(unsigned slot)
{
    for (int i = 0, end = mItems.size(); i < end; i++) {
        ItemInstance *item = mItems.at(i);
        if (item->slot() == slot)
            return i;
    }
    return -1;
}

void InventoryListModel::setItemSlot(unsigned slot, unsigned id,
                                     unsigned amount, unsigned equipmentSlot)
{
    int index = indexBySlot(slot);

    if (index == -1) {
        if (id)
            addItem(slot, id, amount, equipmentSlot);
        return;
    }

    ItemInstance *item = mItems.at(index);
    if (id == item->id()) {
        item->setAmount(amount);
        item->setEquipmentSlot(equipmentSlot);
    } else {
        item = 0;
        removeItem(index);
        if (id)
            addItem(slot, id, amount, equipmentSlot);
    }
}

void InventoryListModel::addItem(unsigned slot, unsigned id,
                                 unsigned amount, unsigned equipmentSlot)
{
    beginInsertRows(QModelIndex(), mItems.size(), mItems.size());
    mItems.append(new ItemInstance(slot, id, amount, equipmentSlot, this));
    endInsertRows();
}

void InventoryListModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    delete mItems.takeAt(index);
    endRemoveRows();
}

void InventoryListModel::removeAllItems()
{
    if (mItems.count() < 1)
        return;

    beginRemoveRows(QModelIndex(), 0, mItems.size());
    qDeleteAll(mItems);
    mItems.clear();
    endRemoveRows();
}
