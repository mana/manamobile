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

#include "abilitylistmodel.h"

#include <QTimerEvent>
#include <QDebug>

using namespace Mana;

Ability::Ability(unsigned id, QObject *parent):
    QObject(parent),
    mId(id),
    mCurrentCharge(0),
    mMaximumCharge(0),
    mRechargeSpeed(0)
{
}

AbilityListModel::AbilityListModel(QObject *parent):
    QAbstractListModel(parent)
{
    mRoleNames.insert(AbilityRole, "ability");
#if QT_VERSION < 0x050000
    setRoleNames(mRoleNames);
#endif

    mAbilitiesUpdateTimer = startTimer(100);
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
                                        unsigned currentCharge,
                                        unsigned maximumCharge,
                                        unsigned chargeSpeed)
{
    qDebug() << "STATUS" << id << currentCharge << maximumCharge << chargeSpeed;

    Ability *ability = mAbilities.value(id);
    bool existed = ability;
    if (!existed)
        ability = new Ability(id, this);

    ability->setStatus(currentCharge, maximumCharge, chargeSpeed);

    if (!existed) {
        beginInsertRows(QModelIndex(), mAbilitiesList.size(),
                        mAbilitiesList.size());
        mAbilities[id] = ability;
        mAbilitiesList.append(ability);
        endInsertRows();
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
}

void AbilityListModel::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != mAbilitiesUpdateTimer)
        return;

    foreach (Ability *ability, mAbilitiesList) {
        unsigned currentCharge = ability->currentCharge();
        currentCharge += ability->rechargeSpeed();
        currentCharge = std::min(currentCharge, ability->maximumCharge());
        ability->setCurrentCharge(currentCharge);
    }
}


