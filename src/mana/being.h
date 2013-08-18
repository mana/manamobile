/*
 * Mana QML plugin
 * Copyright (C) 2010  Thorbjørn Lindeijer 
 * Copyright (C) 2012  Erik Schilling 
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

#ifndef BEING_H
#define BEING_H

#include <QMap>
#include <QObject>
#include <QPointF>
#include <QMetaType>

#include "protocol.h"

#include "resource/action.h"

namespace Mana {

class HairInfo;
class SpriteListModel;

/**
 * Class representing a being.
 */
class Being : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int type READ type CONSTANT)
    Q_PROPERTY(int x READ x NOTIFY positionChanged)
    Q_PROPERTY(int y READ y NOTIFY positionChanged)
    Q_PROPERTY(int direction READ direction NOTIFY directionChanged)
    Q_PROPERTY(int spriteDirection READ spriteDirection NOTIFY directionChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString chatMessage READ chatMessage NOTIFY chatMessageChanged)
    Q_PROPERTY(QString action READ action WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(BeingGender gender READ gender WRITE setGender NOTIFY genderChanged)
    Q_PROPERTY(Mana::SpriteListModel *spriteListModel READ spriteListModel CONSTANT)

    Q_ENUMS(EntityType BeingGender)

public:
    // Keep in sync with protocol
    enum EntityType {
        OBJECT_ITEM = 0,
        OBJECT_ACTOR,
        OBJECT_NPC,
        OBJECT_MONSTER,
        OBJECT_CHARACTER,
        OBJECT_EFFECT,
        OBJECT_OTHER
    };

    // Keep in sync with protocol
    enum BeingGender {
        GENDER_MALE = 0,
        GENDER_FEMALE,
        GENDER_UNSPECIFIED
    };

    Being(int type);

    int type() const { return mType; }

    int id() const { return mId; }
    void setId(int value) { mId = value; }

    int x() const { return mPosition.x(); }
    int y() const { return mPosition.y(); }

    BeingDirection direction() const { return mDirection; }
    Action::SpriteDirection spriteDirection() const;
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

    BeingGender gender() const;
    virtual void setGender(BeingGender gender);

    void lookAt(const QPointF &point);

    SpriteListModel *spriteListModel() const { return mSpriteList; }

signals:
    void positionChanged();
    void directionChanged(BeingDirection newDirection);
    void nameChanged();
    void chatMessageChanged();
    void actionChanged();
    void genderChanged();

protected:
    int mType;
    int mId;
    qreal mWalkSpeed;
    QString mAction;
    QPointF mPosition;
    BeingDirection mDirection;
    QPointF mServerPosition;
    QString mName;
    QString mChatMessage;
    SpriteListModel *mSpriteList;
    Mana::BeingGender mGender;
};

inline Being::BeingGender Being::gender() const
{
    return static_cast<BeingGender>(mGender);
}

inline void Being::setGender(BeingGender gender)
{
    if (mGender != static_cast<Mana::BeingGender>(gender)) {
        mGender = static_cast<Mana::BeingGender>(gender);
        emit genderChanged();
    }
}

} // namespace Mana

Q_DECLARE_METATYPE(Mana::Being*)

#endif // BEING_H
