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

#ifndef ABILITYLISTMODEL_H
#define ABILITYLISTMODEL_H

#include <QAbstractListModel>

namespace Mana {

class Ability : public QObject
{
    Q_OBJECT

    Q_PROPERTY(unsigned id READ id CONSTANT)
    Q_PROPERTY(qreal chargeStatus READ chargeStatus NOTIFY chargeStatusChanged)

public:
    explicit Ability(unsigned id, QObject *parent = 0);

    unsigned id() const;

    unsigned currentCharge() const;
    void setCurrentCharge(unsigned newCharge);

    unsigned maximumCharge() const;
    void setMaximumCharge(unsigned newMaximum);

    unsigned rechargeSpeed() const;
    void setRechargeSpeed(unsigned newSpeed);

    void setStatus(unsigned currentCharge, unsigned maximumCharge,
                   unsigned rechargeSpeed);

    qreal chargeStatus() const;

signals:
    void chargeStatusChanged();

private:
    unsigned mId;
    unsigned mCurrentCharge;
    unsigned mMaximumCharge;
    unsigned mRechargeSpeed;
};

class AbilityListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum SpriteRoles {
        AbilityRole = Qt::UserRole
    };

    explicit AbilityListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void setAbilityStatus(unsigned id, unsigned currentCharge,
                          unsigned maximumCharge, unsigned chargeSpeed);
    void takeAbility(unsigned id);

protected:
    void timerEvent(QTimerEvent *event);

private:
    QMap<int, Ability *> mAbilities;
    QList<Ability *> mAbilitiesList;
    int mAbilitiesUpdateTimer;

    QHash<int, QByteArray> mRoleNames;
};


inline unsigned Ability::id() const
{
    return mId;
}

inline unsigned Ability::currentCharge() const
{
    return mCurrentCharge;
}

inline void Ability::setCurrentCharge(unsigned newCharge)
{
    if (newCharge == mCurrentCharge)
        return;
    mCurrentCharge = newCharge;
    emit chargeStatusChanged();
}

inline unsigned Ability::maximumCharge() const
{
    return mMaximumCharge;
}

inline void Ability::setMaximumCharge(unsigned newMaximum)
{
    mMaximumCharge = newMaximum;
    emit chargeStatusChanged();
}

inline unsigned Ability::rechargeSpeed() const
{
    return mRechargeSpeed;
}

inline void Ability::setRechargeSpeed(unsigned newSpeed)
{
    mRechargeSpeed = newSpeed;
}

inline void Ability::setStatus(unsigned currentCharge, unsigned maximumCharge,
                               unsigned rechargeSpeed)
{
    mCurrentCharge = currentCharge;
    mMaximumCharge = maximumCharge;
    mRechargeSpeed = rechargeSpeed;
    emit chargeStatusChanged();
}

inline qreal Ability::chargeStatus() const
{
    if (mMaximumCharge == 0)
        return 0;
    return (qreal)mCurrentCharge / (qreal)mMaximumCharge;
}

}

Q_DECLARE_METATYPE(Mana::Ability*)

#endif // ABILITYLISTMODEL_H
