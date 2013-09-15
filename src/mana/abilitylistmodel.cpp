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

#include "abilitylistmodel.h"

#include <QTimerEvent>

using namespace Mana;

static const int TIMER_DELAY = 100;

Ability::Ability(unsigned id, unsigned remainingMilliseconds, QObject *parent):
    QObject(parent),
    mId(id),
    mRemainingMilliseconds(remainingMilliseconds)
{
}

AbilityListModel::AbilityListModel(QObject *parent):
    QAbstractListModel(parent)
{
    mRoleNames.insert(AbilityRole, "ability");

    mAbilitiesUpdateTimer = startTimer(TIMER_DELAY);
}

int AbilityListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mAbilitiesList.size();
}

QVariant AbilityListModel::data(const QModelIndex &index, int role) const
{
    if (role == AbilityRole) {
        Ability *ability = mAbilitiesList.at(index.row());
        return QVariant::fromValue(ability);
    }

    return QVariant();
}

QHash<int, QByteArray> AbilityListModel::roleNames() const
{
    return mRoleNames;
}

void AbilityListModel::setAbilityStatus(unsigned id,
                                        unsigned remainingMilliseconds)
{
    Ability *ability = mAbilities.value(id);
    bool existed = ability;
    if (existed)
        ability->setRemainingMilliseconds(remainingMilliseconds);
    else
        ability = new Ability(id, remainingMilliseconds, this);

    if (!existed) {
        beginInsertRows(QModelIndex(), mAbilitiesList.size(),
                        mAbilitiesList.size());
        mAbilities[id] = ability;
        mAbilitiesList.append(ability);
        endInsertRows();
        emit countChanged();
    }
}

void AbilityListModel::takeAbility(unsigned id)
{
    Ability *ability = mAbilities.value(id);
    if (!ability)
        return; // Server sent odd data

    int index = mAbilitiesList.indexOf(ability);
    beginRemoveRows(QModelIndex(), index, index);
    mAbilities.remove(id);
    delete mAbilitiesList.takeAt(index);
    endRemoveRows();
    emit countChanged();
}

void AbilityListModel::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    foreach (Ability *ability, mAbilitiesList) {
        unsigned remainingMilliseconds = ability->remainingMilliseconds();
        remainingMilliseconds -= TIMER_DELAY;
        remainingMilliseconds = std::max(remainingMilliseconds, 0U);
        ability->setRemainingMilliseconds(remainingMilliseconds);
    }
}


