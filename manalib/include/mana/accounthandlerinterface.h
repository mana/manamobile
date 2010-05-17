/*
 *  manalib
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *
 *  This file is part of manalib.
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

#ifndef ACCOUNTHANDLERINTERFACE_H
#define ACCOUNTHANDLERINTERFACE_H

#include <string>

namespace Mana {

class CharacterInfo
{
public:
    std::string name;
    int level;
    int money;
    int slot;
};

class AccountHandlerInterface
{
public:
    virtual void connected() = 0;
    virtual void disconnected() = 0;

    virtual void loginSucceeded() = 0;
    virtual void loginFailed(int error) = 0;

    virtual void characterInfoReceived(const CharacterInfo &info) = 0;
};

} // namespace Mana

#endif // ACCOUNTHANDLERINTERFACE_H
