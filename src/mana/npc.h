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

#ifndef NPC_H
#define NPC_H

#include "being.h"

namespace Mana {

class NPC : public Being
{
    Q_OBJECT

public:
    NPC(int id, QPointF position, int spriteId);

private slots:
    void initializeSprites();

private:
    const int mSpriteId;
};

}

#endif // NPC_H
