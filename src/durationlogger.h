/*
 * Mana Mobile
 * Copyright (C) 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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
