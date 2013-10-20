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

#ifndef ATTRIBUTELISTMODEL_H
#define ATTRIBUTELISTMODEL_H

#include <QAbstractListModel>

namespace Mana {

// Temporary place for having ids for attributes that are required by the client
// TODO: Replace with strings as soon it is implemented.
enum {
    ATTR_MOVE_SPEED_TPS = 16
};

class AttributeValue : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int attributeId READ attributeId CONSTANT)
    Q_PROPERTY(qreal base READ base NOTIFY baseChanged)
    Q_PROPERTY(qreal modified READ modified NOTIFY modifiedChanged)

public:
    explicit AttributeValue(int attributeId, QObject *parent = 0)
        : QObject(parent)
        , mAttributeId(attributeId)
        , mBase(0)
        , mModified(0)
    {}

    int attributeId() const { return mAttributeId; }

    qreal base() const { return mBase; }
    void setBase(qreal value);

    qreal modified() const { return mModified; }
    void setModified(qreal value);

signals:
    void baseChanged();
    void modifiedChanged();

private:
    int mAttributeId;
    qreal mBase;
    qreal mModified;
};

class AttributeListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ length NOTIFY attributeAdded)

public:
    enum AttributeRoles {
        AttributeRole = Qt::UserRole
    };

    explicit AttributeListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int length() const { return rowCount(QModelIndex()); }

    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE Mana::AttributeValue *attribute(int id) const;
    Q_INVOKABLE Mana::AttributeValue *attributeAt(int index) const;

    void setAttribute(int id, qreal base, qreal mod);
    void clear();

    static qreal tpsToPixelsPerSecond(qreal speed);

signals:
    void attributeAdded(int id, Mana::AttributeValue *attribute);

private:
    QMap<int, AttributeValue *> mAttributeValues;
    // Only to allow access by index for the model
    QList<AttributeValue *> mCachedAttributeList;

    QHash<int, QByteArray> mRoleNames;
};

inline qreal AttributeListModel::tpsToPixelsPerSecond(qreal speed)
{
    // TODO: dehardcode the tilesize
    return speed * 32;
}

} // namespace Mana

Q_DECLARE_METATYPE(Mana::AttributeValue *)

#endif // ATTRIBUTELISTMODEL_H
