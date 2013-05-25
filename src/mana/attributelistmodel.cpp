/*
 *  Mana Mobile
 *  Copyright (C) 2013 Erik Schilling
 *
 *  This file is part of Mana Mobile.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#if QT_VERSION < 0x050000
    setRoleNames(mRoleNames);
#endif
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

void AttributeListModel::setAttribute(int id, qreal base, qreal mod)
{
    AttributeValue *value = mAttributeValues.value(id);
    bool needInsert = (value == 0);
    if (needInsert) {
        beginInsertRows(QModelIndex(), mAttributeValues.size(),
                        mAttributeValues.size());
        value = mAttributeValues[id] = new AttributeValue(id, this);
        mCachedAttributeList.push_back(value);
    }
    value->setBase(base);
    value->setModified(mod);

    if (needInsert)
        endInsertRows();
}
