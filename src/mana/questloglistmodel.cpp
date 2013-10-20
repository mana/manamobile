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

#include "questloglistmodel.h"

using namespace Mana;

Quest::Quest(int id, QObject *parent)
    : QObject(parent)
    , mId(id)
    , mState(OPEN)
{
}

void Quest::setState(Quest::State state)
{
    if (mState != state) {
        mState = state;
        emit stateChanged();
    }
}

void Quest::setTitle(const QString &value)
{
    if (mTitle != value) {
        mTitle = value;
        emit titleChanged();
    }
}

void Quest::setDescription(const QString &value)
{
    if (mDescription != value) {
        mDescription = value;
        emit descriptionChanged();
    }
}


QuestlogListModel::QuestlogListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    mRoleNames.insert(QuestRole, "quest");
}

int QuestlogListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mQuestList.count();
}

QVariant QuestlogListModel::data(const QModelIndex &index, int role) const
{
    if (role == QuestRole) {
        Quest *quest = mQuestList.at(index.row());
        return QVariant::fromValue(quest);
    }

    return QVariant();
}

QHash<int, QByteArray> QuestlogListModel::roleNames() const
{
    return mRoleNames;
}

Quest *QuestlogListModel::createOrGetQuest(int id)
{
    QMap<int, Quest *>::iterator it = mQuests.find(id);
    if (it != mQuests.end())
        return it.value();

    beginInsertRows(QModelIndex(), mQuestList.size(),
                    mQuestList.size());

    Quest *quest = new Quest(id, this);
    mQuests[id] = quest;
    mQuestList.append(quest);
    endInsertRows();

    return quest;
}
