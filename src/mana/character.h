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

class AttributeValue : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double base READ base NOTIFY baseChanged)
    Q_PROPERTY(double modified READ modified NOTIFY modifiedChanged)

public:
    explicit AttributeValue(QObject *parent = 0)
        : QObject(parent)
        , mBase(0)
        , mModified(0)
    {}

    double base() const { return mBase; }
    void setBase(double value);

    double modified() const { return mModified; }
    void setModified(double value);

signals:
    void baseChanged();
    void modifiedChanged();

private:
    double mBase;
    double mModified;
};

class Character : public Being
{
    Q_OBJECT

public:
    enum SlotLayer {
        SLOT_RACE = 0,
        SLOT_HAIR,
        SLOT_EQUIPMENT
    };

    Character();

    void setEquipmentSlot(int slot, int id);

    QMap<int, int> &equipmentSlots() { return mEquipmentSlots; }

    void setHairStyle(int style, int color);
    int hairStyle() const { return mHairStyle; }
    int hairColor() const { return mHairColor; }

    virtual void setGender(BeingGender gender);

    void setCharacterSlot(int value) { mCharacterSlot = value; }
    int characterSlot() const { return mCharacterSlot; }

    void setLevel(int value) { mLevel = value; }
    int level() const { return mLevel; }

    void setAttribute(int id, double base, double mod);

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

    /* Only used for the characters created for the character selection screen */
    int mCharacterSlot;
    int mLevel;
    QMap<int, AttributeValue *> mAttributes;
};

}

Q_DECLARE_METATYPE(Mana::Character*)

#endif // CHARACTER_H
