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

#ifndef QUESTLOGLISTMODEL_H
#define QUESTLOGLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

namespace Mana {

class Quest : public QObject
{
    Q_OBJECT

    Q_ENUMS(State)

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)

public:
    enum State {
        OPEN,
        FINISHED,
        FAILED
    };

    Quest(int id, QObject *parent);

    int id() const;

    State state() const;
    void setState(State state);

    const QString &title() const;
    void setTitle(const QString &value);

    const QString &description() const;
    void setDescription(const QString &value);

signals:
    void stateChanged();
    void titleChanged();
    void descriptionChanged();

private:
    int mId;
    State mState;
    QString mTitle;
    QString mDescription;
};

class QuestlogListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum SpriteRoles {
        QuestRole = Qt::UserRole
    };

    explicit QuestlogListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Quest *createOrGetQuest(int id);
    void clear();

private:
    QMap<int, Quest *> mQuests;
    QList<Quest *> mQuestList;

    QHash<int, QByteArray> mRoleNames;
};

inline int Quest::id() const
{
    return mId;
}

inline Quest::State Quest::state() const
{
    return mState;
}

inline const QString &Quest::title() const
{
    return mTitle;
}

inline const QString &Quest::description() const
{
    return mDescription;
}

}

Q_DECLARE_METATYPE(Mana::Quest*)

#endif // QUESTLOGLISTMODEL_H
