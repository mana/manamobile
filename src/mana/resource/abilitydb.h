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

#ifndef ABILITYDB_H
#define ABILITYDB_H

#include <QMap>
#include <QObject>

namespace Mana {

class AbilityInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(AbilityTargetType targetType READ targetType CONSTANT)
    Q_PROPERTY(QString useAction READ useAction CONSTANT)

    Q_ENUMS(AbilityTargetType)

public:
    enum AbilityTargetType {
        TARGET_BEING,
        TARGET_POINT,
        TARGET_DIRECTION
    };

    explicit AbilityInfo(int id, QObject *parent = 0);

    int id() const;

    QString name() const;
    void setName(const QString &name);

    AbilityTargetType targetType() const;
    void setTargetType(AbilityTargetType type);

    const QString &useAction() const;
    void setUseAction(const QString &actionName);

private:
    int mId;
    QString mName;
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

    Q_INVOKABLE Mana::AbilityInfo *getInfo(int id) const
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

Q_DECLARE_METATYPE(Mana::AbilityInfo*)

#endif // ABILITYDB_H
