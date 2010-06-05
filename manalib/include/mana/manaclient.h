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

#ifndef LIBMANA_MANACLIENT_H
#define LIBMANA_MANACLIENT_H

#include <string>

namespace Mana {

class AccountHandlerInterface;
class CharacterInfo;
class ManaClientPrivate;

/**
 * A combination of host and port.
 */
class ServerAddress
{
public:
    std::string host;
    unsigned short port;

    bool equals(const ServerAddress &other) const
    {
        return host == other.host
                && port == other.port;
    }

    inline bool operator ==(const ServerAddress &other) const
    { return equals(other); }

    inline bool operator !=(const ServerAddress &other) const
    { return !equals(other); }
};

/**
 * The main object representing a Mana client. Allows interacting with the
 * different parts of the Mana server.
 */
class ManaClient
{
public:
    ManaClient();
    ~ManaClient();

    void setAccountHandler(AccountHandlerInterface *);

    /**
     * Handles incoming and outgoing network traffic. Call this method
     * frequently (10 times per second should be fine, since the server
     * also does it at that frequency).
     */
    void handleNetworkTraffic();

    void connectToAccountServer(const ServerAddress &address);
    void disconnectFromAccountServer();
    bool isConnectedToAccountServer() const;

    std::string updateHost() const;

    void login(const std::string &username,
               const std::string &password);

    void chooseCharacter(const CharacterInfo &character);

private:
    ManaClientPrivate *d;
};

} // namespace Mana

#endif // LIBMANA_MANACLIENT_H
