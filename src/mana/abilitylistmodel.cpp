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

#include "resource/abilitydb.h"

using namespace Mana;

static const int TIMER_DELAY = 100;

Ability::Ability(unsigned id, unsigned remainingMilliseconds, QObject *parent):
    QObject(parent),
    mId(id),
    mRemainingMilliseconds(remainingMilliseconds)
{
    AbilityDB *abilityDb = AbilityDB::instance();
    if (!abilityDb->isLoaded())
        connect(abilityDb, SIGNAL(loaded()), this, SLOT(sortByPriority()));
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

static bool abilityLessThan(Ability *lhs, Ability *rhs)
{
    AbilityDB *db = AbilityDB::instance();
    AbilityInfo *lhsInfo = db->getInfo(lhs->id());
    AbilityInfo *rhsInfo = db->getInfo(rhs->id());

    return lhsInfo->priority() < rhsInfo->priority();
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
        int index;
        if (AbilityDB::instance()->isLoaded()) {
            QList<Ability *>::iterator it = qLowerBound(mAbilitiesList.begin(),
                                                        mAbilitiesList.end(),
                                                        ability,
                                                        abilityLessThan);
            index = it - mAbilitiesList.begin();
        } else {
            index = mAbilitiesList.size();
        }
        beginInsertRows(QModelIndex(), index, index);
        mAbilitiesList.insert(index, ability);
        mAbilities[id] = ability;
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

void AbilityListModel::clear()
{
    if (mAbilitiesList.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), 0, mAbilitiesList.size() - 1);
    qDeleteAll(mAbilitiesList);
    mAbilitiesList.clear();
    mAbilities.clear();
    endRemoveRows();
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

void AbilityListModel::sortByPriority()
{
    beginResetModel();
    qSort(mAbilitiesList.begin(), mAbilitiesList.end(), abilityLessThan);
    endResetModel();
}


