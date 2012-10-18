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

#ifndef BEINGITEM_H
#define BEINGITEM_H

#include "mana/protocol.h"

#include "mana/resource/resource.h"
#include "mana/resource/spritedef.h"


#include <QDeclarativeItem>

class SpriteItem;

namespace Mana {
class Being;
}

class BeingItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(Mana::Being *being READ being WRITE setBeing)

public:
    BeingItem(QDeclarativeItem *parent = 0);
    ~BeingItem();

    Mana::Being *being() { return mBeing; }
    void setBeing(Mana::Being *being);

private slots:
    void slotEquipped(int slot, int itemId);
    void slotUnequipping(int slot);
    void actionChanged(const QString &newAction);
    void directionChanged(Mana::BeingDirection newDirection);


private:
    Mana::Being *mBeing;

    QMap<int, SpriteItem *> mSprites;

};

#endif // BEINGITEM_H
