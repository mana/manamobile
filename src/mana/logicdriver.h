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

#ifndef MANA_LOGICDRIVER_H
#define MANA_LOGICDRIVER_H

#include <QAbstractAnimation>

namespace Mana {

class LogicDriver : public QAbstractAnimation
{
    Q_OBJECT

public:
    explicit LogicDriver(QObject *parent = 0);

    int duration() const;

signals:
    /**
     * Emitted every time a logic update should be made. \a deltaTime is in
     * seconds.
     */
    void update(qreal deltaTime);

protected:
    void updateCurrentTime(int currentTime);
};

} // namespace Mana

#endif // MANA_LOGICDRIVER_H
