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

#ifndef DROPLISTMODEL_H
#define DROPLISTMODEL_H

#include <QAbstractListModel>
#include <QPoint>

namespace Mana {

class Drop : public QObject
{
    Q_OBJECT

    Q_PROPERTY(unsigned id READ id CONSTANT)
    Q_PROPERTY(QPoint position READ position CONSTANT)

public:
    explicit Drop(unsigned id, const QPoint &position, QObject *parent);

    unsigned id() const;

    const QPoint &position() const;

private:
    unsigned mId;
    QPoint mPosition;
};

class DropListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum SpriteRoles {
        DropRole = Qt::UserRole
    };

    explicit DropListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void addDrop(unsigned id, const QPoint &position);
    void removeDrop(const QPoint &coordinates);
    void clear();

private:
    QList<Drop *> mDropList;

    QHash<int, QByteArray> mRoleNames;
};


inline Drop::Drop(unsigned id, const QPoint &position, QObject *parent)
    : QObject(parent)
    , mId(id)
    , mPosition(position)
{
}

inline unsigned Drop::id() const
{
    return mId;
}

inline const QPoint &Drop::position() const
{
    return mPosition;
}

}

Q_DECLARE_METATYPE(Mana::Drop*)

#endif // DROPLISTMODEL_H
