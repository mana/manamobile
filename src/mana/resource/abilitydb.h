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

#ifndef ABILITYDB_H
#define ABILITYDB_H

#include <QList>
#include <QMap>
#include <QObject>

#include "spritedef.h"

#include "mana/protocol.h"

class QNetworkReply;

namespace Mana {

class AbilityInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString category READ category CONSTANT)
    Q_PROPERTY(bool rechargable READ rechargable CONSTANT)
    Q_PROPERTY(int rechargeSpeed READ rechargeSpeed CONSTANT)
    Q_PROPERTY(AbilityTargetType targetType READ targetType CONSTANT)
    Q_PROPERTY(QString useAction READ useAction CONSTANT)

    Q_ENUMS(AbilityTargetType)

public:
    enum AbilityTargetType {
        TARGET_BEING,
        TARGET_POINT
    };

    explicit AbilityInfo(int id, QObject *parent = 0);

    int id() const;

    QString name() const;
    void setName(const QString &name);

    QString category() const;
    void setCategory(QString name);

    bool rechargable() const;
    void setRechargable(bool rechargable);

    unsigned neededPoints() const;
    void setNeededPoints(unsigned points);

    unsigned rechargeSpeed() const;
    void setRechargeSpeed(unsigned speed);

    AbilityTargetType targetType() const;
    void setTargetType(AbilityTargetType type);

    const QString &useAction() const;
    void setUseAction(const QString &actionName);

private:
    int mId;
    QString mName;
    QString mCategory;
    bool mRechargable;
    unsigned mNeededPoints;
    unsigned mRechargeSpeed;
    AbilityTargetType mTargetType;
    QString mUseAction;
};

class AbilityDB : public QObject
{
    Q_OBJECT

public:
    AbilityDB(QObject *parent);

    Q_INVOKABLE void load();
    Q_INVOKABLE void unload();
    bool isLoaded() const { return mLoaded; }

    Q_INVOKABLE const AbilityInfo *getInfo(int id) const
    { return mAbilities[id]; }

    void setInfo(int id, AbilityInfo *info)
    { mAbilities[id] = info; }

    QMap<unsigned, AbilityInfo *> abilities() { return mAbilities; }

    static AbilityDB *instance() { return mInstance; }

signals:
    void abilitiesChanged();
    void loaded();

private slots:
    void fileReady();

private:
    static AbilityDB *mInstance;

    bool mLoaded;
    ~AbilityDB() { unload(); }

    QMap<unsigned, AbilityInfo *> mAbilities;
};


inline AbilityInfo::AbilityInfo(int id, QObject *parent):
    QObject(parent),
    mId(id)
{}

inline int AbilityInfo::id() const
{
    return mId;
}

inline QString AbilityInfo::name() const
{
    return mName;
}

inline void AbilityInfo::setName(const QString &name)
{
    mName = name;
}

inline QString AbilityInfo::category() const
{
    return mCategory;
}

inline void AbilityInfo::setCategory(QString name)
{
    mCategory = name;
}

inline bool AbilityInfo::rechargable() const
{
    return mRechargable;
}

inline void AbilityInfo::setRechargable(bool rechargable)
{
    mRechargable = rechargable;
}

inline unsigned AbilityInfo::neededPoints() const
{
    return mNeededPoints;
}

inline void AbilityInfo::setNeededPoints(unsigned points)
{
    mNeededPoints = points;
}

inline unsigned AbilityInfo::rechargeSpeed() const
{
    return mRechargeSpeed;
}

inline void AbilityInfo::setRechargeSpeed(unsigned speed)
{
    mRechargeSpeed = speed;
}

inline AbilityInfo::AbilityTargetType AbilityInfo::targetType() const
{
    return mTargetType;
}

inline void AbilityInfo::setTargetType(AbilityTargetType type)
{
    mTargetType = type;
}

inline const QString &AbilityInfo::useAction() const
{
    return mUseAction;
}

inline void AbilityInfo::setUseAction(const QString &actionName)
{
    mUseAction = actionName;
}

} // namespace Mana

#endif // ABILITYDB_H
