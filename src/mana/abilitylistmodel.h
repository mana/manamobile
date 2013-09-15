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
    Q_PROPERTY(unsigned remainingMilliseconds READ remainingMilliseconds NOTIFY remainingMillisecondsChanged)

public:
    explicit Ability(unsigned id, unsigned remainingTicks, QObject *parent = 0);

    unsigned id() const;

    unsigned remainingMilliseconds() const;
    void setRemainingMilliseconds(unsigned remainingMilliseconds);

signals:
    void remainingMillisecondsChanged();

private:
    unsigned mId;
    unsigned mRemainingMilliseconds;
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

    void setAbilityStatus(unsigned id, unsigned remainingMilliseconds);
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

inline unsigned Ability::remainingMilliseconds() const
{
    return mRemainingMilliseconds;
}

inline void Ability::setRemainingMilliseconds(unsigned milliseconds)
{
    if (mRemainingMilliseconds == milliseconds)
        return;
    mRemainingMilliseconds = milliseconds;
    emit remainingMillisecondsChanged();
}

}

Q_DECLARE_METATYPE(Mana::Ability*)

#endif // ABILITYLISTMODEL_H
