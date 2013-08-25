/*
 * Mana QML plugin
 * Copyright (C) 2013  Thorbjørn Lindeijer
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

#include "logicdriver.h"

namespace Mana {

LogicDriver::LogicDriver(QObject *parent) :
    QAbstractAnimation(parent)
{
    setLoopCount(-1); // loop forever
}

int LogicDriver::duration() const
{
    return 1000;
}

void LogicDriver::updateCurrentTime(int currentTime)
{
    static int lastTime = currentTime;

    int elapsed = currentTime - lastTime;
    if (elapsed < 0)
        elapsed += 1000;

    lastTime = currentTime;

    emit update(elapsed / qreal(1000));
}

} // namespace Mana
