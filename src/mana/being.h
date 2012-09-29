/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#include <QObject>
#include <QPointF>

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

public:
    enum Action {
        Stand,
        Move,
        Attack,
        Sit,
        Dead,
        Hurt
    };

    Being(int type, int id, QPointF position);

    int type() const { return mType; }
    int id() const { return mId; }
    int x() const { return mPosition.x(); }
    int y() const { return mPosition.y(); }

    int direction() const { return mDirection; }
    void setDirection(int direction);

    QString name() const { return mName; }
    void setName(const QString &name);

    QString chatMessage() const { return mChatMessage; }

    qreal walkSpeed() const { return mWalkSpeed; }
    void setWalkSpeed(qreal walkSpeed) { mWalkSpeed = walkSpeed; }

    Action action() const { return mAction; }
    void setAction(Action action) { mAction = action; }

    QPointF position() const { return mPosition; }
    void setPosition(QPointF position);

    QPointF serverPosition() const { return mServerPosition; }
    void setServerPosition(QPointF position);

    void say(const QString &text);

signals:
    void positionChanged();
    void directionChanged();
    void nameChanged();
    void chatMessageChanged();

private:
    int mType;
    int mId;
    qreal mWalkSpeed;
    Action mAction;
    QPointF mPosition;
    int mDirection;
    QPointF mServerPosition;
    QString mName;
    QString mChatMessage;
};

} // namespace Mana

#endif // BEING_H
