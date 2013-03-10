/*
 * Mana QML plugin
 * Copyright (C) 2012  Erik Schilling
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

#ifndef CHARACTER_H
#define CHARACTER_H

#include "being.h"

namespace Mana {

class Character : public Being
{
    Q_OBJECT

    Q_PROPERTY(int hairStyle READ hairStyle WRITE setHairStyle NOTIFY hairChanged)
    Q_PROPERTY(int hairColor READ hairColor WRITE setHairColor NOTIFY hairChanged)

public:
    enum SlotLayer {
        SLOT_RACE = 0,
        SLOT_HAIR,
        SLOT_EQUIPMENT
    };

    Character();

    void setEquipmentSlots(const QMap<int, int> &equipmentSlots);

    QMap<int, int> &equipmentSlots() { return mEquipmentSlots; }

    void setHairStyle(int style, int color);

    int hairStyle() const { return mHairStyle; }
    void setHairStyle(int style);

    int hairColor() const { return mHairColor; }
    void setHairColor(int color);

    virtual void setGender(BeingGender gender);

    void setCharacterSlot(int value) { mCharacterSlot = value; }
    int characterSlot() const { return mCharacterSlot; }

    void setLevel(int value) { mLevel = value; }
    int level() const { return mLevel; }

signals:
    void hairChanged();

private slots:
    void rebuildSprites();

private:
    void addItemSprites(int slot, int itemId);

    QMap<int, int> mEquipmentSlots;
    int mHairStyle;
    int mHairColor;

    /* Only used for the characters created for the character selection screen */
    int mCharacterSlot;
    int mLevel;
};

inline void Character::setHairStyle(int style)
{
    setHairStyle(style, mHairColor);
}

inline void Character::setHairColor(int color)
{
    setHairStyle(mHairStyle, color);
}

} // namespace Mana

Q_DECLARE_METATYPE(Mana::Character*)

#endif // CHARACTER_H
