/*
 *  Mana Mobile
 *  Copyright (C) 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *
 *  This file is part of Mana Mobile.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DURATIONLOGGER_H
#define DURATIONLOGGER_H

#include <QTime>

/**
 * A helper class that logs the duration of a specific scope.
 */
class DurationLogger
{
public:
    DurationLogger(const char *thing) : thing(thing) { time.start(); }
    ~DurationLogger() { qDebug() << thing << time.elapsed(); }
private:
    QTime time;
    const char *thing;
};

/**
 * Use this macro in functions to conveniently time their duration.
 */
#define LOG_DURATION DurationLogger _durationLogger(Q_FUNC_INFO)

#endif // DURATIONLOGGER_H
