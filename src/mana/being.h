/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#ifndef BEING_H
#define BEING_H

#include <QMap>
#include <QObject>
#include <QPointF>
#include <QMetaType>

#include "protocol.h"

namespace Mana {

/**
 * Class representing a being.
 */
class Being : public QObject
{
    Q_OBJECT

    Q_ENUMS(Action)
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int x READ x NOTIFY positionChanged)
    Q_PROPERTY(int y READ y NOTIFY positionChanged)
    Q_PROPERTY(int direction READ direction NOTIFY directionChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString chatMessage READ chatMessage NOTIFY chatMessageChanged)
    Q_PROPERTY(BeingGender gender READ gender CONSTANT)

public:
    Being(int type, int id, QPointF position);

    int type() const { return mType; }
    int id() const { return mId; }
    int x() const { return mPosition.x(); }
    int y() const { return mPosition.y(); }

    BeingDirection direction() const { return mDirection; }
    void setDirection(BeingDirection direction);

    QString name() const { return mName; }
    void setName(const QString &name);

    QString chatMessage() const { return mChatMessage; }

    qreal walkSpeed() const { return mWalkSpeed; }
    void setWalkSpeed(qreal walkSpeed) { mWalkSpeed = walkSpeed; }

    const QString &action() const { return mAction; }
    void setAction(const QString &action);

    QPointF position() const { return mPosition; }
    void setPosition(QPointF position);

    QPointF serverPosition() const { return mServerPosition; }
    void setServerPosition(QPointF position);

    void say(const QString &text);

    BeingGender gender() const { return mGender; }
    void setGender(BeingGender gender) { mGender = gender; }

    void setSprite(int slot, int id);

    QMap<int, int> &equipmentSlots() { return mSlots; }

    void lookAt(const QPointF &point);

signals:
    void positionChanged();
    void directionChanged(Mana::BeingDirection newDirection);
    void nameChanged();
    void chatMessageChanged();
    void actionChanged(const QString &newAction);
    void slotUnequipping(int slot);
    void slotEquipped(int slot, int itemId);

private:
    int mType;
    int mId;
    qreal mWalkSpeed;
    QString mAction;
    QPointF mPosition;
    BeingDirection mDirection;
    QPointF mServerPosition;
    QString mName;
    QString mChatMessage;
    BeingGender mGender;

    // key: equipment slot
    // value: item id
    QMap<int, int> mSlots;
};

} // namespace Mana

Q_DECLARE_METATYPE(Mana::Being*)

#endif // BEING_H
