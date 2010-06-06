/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#ifndef ACCOUNTCLIENT_H
#define ACCOUNTCLIENT_H

#include "enetclient.h"

namespace Mana {

class AccountHandlerInterface;

namespace Internal {

/**
 * The account client allows interacting with the account server.
 */
class AccountClient : public ENetClient
{
public:
    AccountClient()
        : mAccountHandler(0)
        , mGameServerPort(0)
        , mChatServerPort(0)
    {}

    void setAccountHandler(AccountHandlerInterface *handler)
    { mAccountHandler = handler; }

    std::string updateHost() const { return mUpdateHost; }
    std::string token() const { return mToken; }
    std::string gameServerHost() const { return mGameServerHost; }
    std::string chatServerHost() const { return mChatServerHost; }
    unsigned short gameServerPort() const { return mGameServerPort; }
    unsigned short chatServerPort() const { return mChatServerPort; }

protected:
    void connected();
    void disconnected();
    void messageReceived(MessageIn &message);

private:
    void handleLoginResponse(MessageIn &message);
    void handleCharacterInfo(MessageIn &message);
    void handleCharacterSelectResponse(MessageIn &message);

    AccountHandlerInterface *mAccountHandler;

    std::string mUpdateHost;
    std::string mToken;
    std::string mGameServerHost;
    std::string mChatServerHost;
    unsigned short mGameServerPort;
    unsigned short mChatServerPort;
};

} // namespace Internal
} // namespace Mana

#endif // ACCOUNTCLIENT_H
