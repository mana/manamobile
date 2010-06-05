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

#include <mana/manaclient.h>

#include <mana/accounthandlerinterface.h>
#include <mana/protocol.h>

#include "accountclient.h"
#include "chatclient.h"
#include "gameclient.h"
#include "messageout.h"
#include "sha256.h"

using namespace Mana::Internal;

namespace Mana {

class ManaClientPrivate
{
public:
    ManaClientPrivate()
        : accountClient(new AccountClient)
        , chatClient(new ChatClient)
        , gameClient(new GameClient)
    {}

    ~ManaClientPrivate()
    {
        delete accountClient;
        delete chatClient;
        delete gameClient;
    }

    AccountClient *accountClient;
    ChatClient *chatClient;
    GameClient *gameClient;

    std::string updateHost;
};


ManaClient::ManaClient()
    : d(new ManaClientPrivate)
{
}

ManaClient::~ManaClient()
{
    delete d;
}

void ManaClient::setAccountHandler(AccountHandlerInterface *handler)
{
    d->accountClient->setAccountHandler(handler);
}

void ManaClient::handleNetworkTraffic()
{
    d->accountClient->service();
    d->chatClient->service();
    d->gameClient->service();
}

void ManaClient::connectToAccountServer(const ServerAddress &address)
{
    d->accountClient->connect(address.host.c_str(), address.port);
}

void ManaClient::disconnectFromAccountServer()
{
    d->accountClient->disconnect();
}

bool ManaClient::isConnectedToAccountServer() const
{
    return d->accountClient->isConnected();
}

std::string ManaClient::updateHost() const
{
    return d->updateHost;
}

void ManaClient::login(const std::string &username,
                       const std::string &password)
{
    const std::string hashedPassword = sha256(username + password);

    MessageOut loginMessage(PAMSG_LOGIN);
    loginMessage.writeInt32(0); // client version
    loginMessage.writeString(username);
    loginMessage.writeString(hashedPassword);
    d->accountClient->send(loginMessage);
}

void ManaClient::chooseCharacter(const CharacterInfo &character)
{
    MessageOut m(PAMSG_CHAR_SELECT);
    m.writeInt8(character.slot);
    d->accountClient->send(m);
}

} // namespace Mana
