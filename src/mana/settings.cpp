/*
 *  settings.cpp
 *  Copyright (C) 2011, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *  Copyright (C) 2011, seeseekey <seeseekey@googlemail.com>
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

#include "settings.h"

namespace Mana {

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    mSettings.setValue(key, value);
}

QVariant Settings::value(const QString &key,
                         const QVariant &defaultValue) const
{
    return mSettings.value(key, defaultValue);
}

} // namespace Mana
