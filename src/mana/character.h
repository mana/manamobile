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

#ifndef CHARACTER_H
#define CHARACTER_H

#include "being.h"

namespace Mana {

class Character : public Being
{
    Q_OBJECT

public:
    enum SlotLayer {
        SLOT_RACE = 0,
        SLOT_HAIR,
        SLOT_EQUIPMENT
    };

    Character(int id, QPointF position);

    void setEquipmentSlot(int slot, int id);

    QMap<int, int> &equipmentSlots() { return mEquipmentSlots; }

    void setHairStyle(int style, int color);
    int hairStyle() const { return mHairStyle; }
    int hairColor() const { return mHairColor; }

    virtual void setGender(BeingGender gender);

signals:
    void slotUnequipping(int slot);
    void slotEquipped(int slot, int itemId);
    void hairChanged();

private slots:
    void rebuildSprites();

private:
    QMap<int, int> mEquipmentSlots;
    int mHairStyle;
    int mHairColor;
};

}

#endif // CHARACTER_H
