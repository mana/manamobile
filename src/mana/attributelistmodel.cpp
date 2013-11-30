/*
 * Mana Mobile
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

#include "attributelistmodel.h"

using namespace Mana;

void AttributeValue::setBase(qreal value)
{
    if (mBase != value) {
        mBase = value;
        emit baseChanged();
    }
}

void AttributeValue::setModified(qreal value)
{
    if (mModified != value) {
        mModified = value;
        emit modifiedChanged();
    }
}

AttributeListModel::AttributeListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    mRoleNames.insert(AttributeRole, "value");
}

int AttributeListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mCachedAttributeList.size();
}

QVariant AttributeListModel::data(const QModelIndex &index, int role) const
{
    if (role == AttributeRole) {
        AttributeValue *attributeValue = mCachedAttributeList.at(index.row());
        mAttributeValues.values();
        return QVariant::fromValue(attributeValue);
    }

    return QVariant();
}

QHash<int, QByteArray> AttributeListModel::roleNames() const
{
    return mRoleNames;
}

AttributeValue *AttributeListModel::attribute(int id) const
{
    return mAttributeValues.value(id);
}

AttributeValue *AttributeListModel::attributeAt(int index) const
{
    return mCachedAttributeList.at(index);
}

void AttributeListModel::setAttribute(int id, qreal base, qreal mod)
{
    AttributeValue *value = mAttributeValues.value(id);
    bool needInsert = (value == 0);
    if (needInsert) {
        beginInsertRows(QModelIndex(), mAttributeValues.size(),
                        mAttributeValues.size());
        value = mAttributeValues[id] = new AttributeValue(id, this);
        mCachedAttributeList.append(value);
    }
    value->setBase(base);
    value->setModified(mod);

    if (needInsert) {
        endInsertRows();
        emit attributeAdded(id, value);
    }
}

void AttributeListModel::clear()
{
    if (mCachedAttributeList.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), 0, mCachedAttributeList.size() - 1);
    qDeleteAll(mCachedAttributeList);
    mCachedAttributeList.clear();
    mAttributeValues.clear();
    endRemoveRows();
}
