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

#ifndef ACTION_H
#define ACTION_H

#include <QMap>
#include <QObject>

namespace Mana {

class Animation;

class Action : public QObject
{
    Q_OBJECT

public:
    explicit Action(QObject *parent = 0);
    ~Action();

    void setAnimation(int direction, Animation *animation);

    const Animation *animation(int direction) const;

private:
    QMap<int, Animation *> mAnimations;

};

}

#endif // ACTION_H
