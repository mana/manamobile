/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#ifndef BEINGLISTMODEL_H
#define BEINGLISTMODEL_H

#include <QAbstractListModel>

namespace Mana {

class Being;
class Character;
class MessageIn;

class BeingListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(BeingRoles)

public:
    enum BeingRoles {
        BeingRole = Qt::UserRole
    };

    enum SpriteLayer
    {
        SPRITE_LAYER_BASE = 0,
        SPRITE_LAYER_HAIR,
        FIXED_SPRITE_LAYER_SIZE
    };

    explicit BeingListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QString playerName() const { return mPlayerName; }
    void setPlayerName(const QString &name) { mPlayerName = name; }

    Being *player() const { return mPlayerBeing; }

    void handleBeingEnter(MessageIn &message);
    void handleBeingLeave(MessageIn &message);
    void handleBeingDirChange(MessageIn &message);
    void handleBeingsMove(MessageIn &message);
    void handleBeingLooksChange(MessageIn &message);
    void handleBeingActionChange(MessageIn &message);
    void handleBeingSay(MessageIn &message);

signals:
    void playerChanged();

protected:
    void timerEvent(QTimerEvent *);

private:
    Being *beingAt(int index) const { return mBeings.at(index); }
    Being *beingById(int id) const;
    void addBeing(Being *being);
    void removeBeing(int id);
    int indexOfBeing(int id) const;
    const QList<Being*> &beings() const { return mBeings; }

    void handleLooks(Character *being, MessageIn &message);
    void handleHair(Character *ch, MessageIn &message);

    QList<Being*> mBeings;

    int mBeingUpdateTimer;
    QString mPlayerName;
    Being *mPlayerBeing;
};

} // namespace Mana

#endif // BEINGLISTMODEL
