/*
 * manalib
 * Copyright 2013, Erik Schilling <ablu.erikschilling@googlemail.com>
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

#ifndef ATTRIBUTEDB_H
#define ATTRIBUTEDB_H

#include "mana/protocol.h"

#include <QList>
#include <QMap>
#include <QMetaType>
#include <QObject>

#include <limits>

class XmlReader;

class QNetworkReply;

namespace Mana {

class AttributeInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(bool modifiable READ modifiable CONSTANT)
    Q_PROPERTY(qreal min READ min CONSTANT)
    Q_PROPERTY(qreal max READ max CONSTANT)

public:
    AttributeInfo(int id,
                  QString name,
                  QString description,
                  bool modifiable,
                  qreal min,
                  qreal max,
                  QObject *parent)
        : QObject(parent)
        , mId(id)
        , mName(name)
        , mDescription(description)
        , mModifiable(modifiable)
        , mMinimum(min)
        , mMaximum(max)
    {}

    int id() const { return mId; }
    QString name() const { return mName; }
    QString description() const { return mDescription; }
    bool modifiable() const { return mModifiable; }
    qreal min() const { return mMinimum; }
    qreal max() const { return mMaximum; }

private:
    int mId;
    QString mName;
    QString mDescription;
    bool mModifiable;
    qreal mMinimum;
    qreal mMaximum;
};

class AttributeDB : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY attributesChanged)

public:
    explicit AttributeDB(QObject *parent);

    Q_INVOKABLE void load();
    Q_INVOKABLE void unload();
    bool isLoaded() const { return mLoaded; }

    Q_INVOKABLE Mana::AttributeInfo *getInfo(int id) const
    { return mAttributes.value(id); }

    static AttributeDB *instance() { return mInstance; }

signals:
    void attributesChanged();
    void loaded();

private slots:
    void fileReady();

private:
    static AttributeDB *mInstance;

    bool mLoaded;

    QMap<int, AttributeInfo *> mAttributes;
};

} // namespace Mana

Q_DECLARE_METATYPE(Mana::AttributeInfo*)

#endif // ATTRIBUTEDB_H
