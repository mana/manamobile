/*
 * settings.cpp
 * Copyright (C) 2011  Thorbjørn Lindeijer 
 * Copyright (C) 2011  seeseekey 
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
