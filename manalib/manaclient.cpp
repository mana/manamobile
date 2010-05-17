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

#include "enetclient.h"
#include "messagein.h"
#include "messageout.h"
#include "sha256.h"

#include <enet/enet.h>

#include <stdio.h>

namespace Mana {

namespace {

class AccountClientEventHandler : public ENetClientEventHandler
{
public:
    AccountClientEventHandler() : mAccountHandler(0) {}

    void setAccountHandler(AccountHandlerInterface *handler)
    { mAccountHandler = handler; }


    // ENetClientEventHandler implementation

    void connected()
    { mAccountHandler->connected(); }

    void disconnected()
    { mAccountHandler->disconnected(); }

    void messageReceived(MessageIn &message);

private:
    void handleLoginResponse(MessageIn &message);
    void handleCharacterInfo(MessageIn &message);

    AccountHandlerInterface *mAccountHandler;
};

void AccountClientEventHandler::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case APMSG_LOGIN_RESPONSE:
        handleLoginResponse(message);
        break;
    case APMSG_CHAR_INFO:
        handleCharacterInfo(message);
        break;
    case XXMSG_INVALID:
        printf("Invalid received! Did we send an invalid message?\n");
        break;
    default:
        printf("Unknown message %d\n", message.id());
        break;
    }
}

void AccountClientEventHandler::handleLoginResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK)
        mAccountHandler->loginSucceeded();
    else
        mAccountHandler->loginFailed(error);
}

void AccountClientEventHandler::handleCharacterInfo(MessageIn &message)
{
    CharacterInfo info;

    info.slot = message.readInt8();
    info.name = message.readString();
    message.readInt8(); // gender
    message.readInt8(); // hair style
    message.readInt8(); // hair color
    message.readInt16(); // level
    message.readInt16(); // character points
    message.readInt16(); // correction points
    info.money = message.readInt32();
    for (int i = 0; i < 7; i++)
    {
        message.readInt8(); // attribute
    }

    mAccountHandler->characterInfoReceived(info);
}

} // anonymous namespace


class ManaClientPrivate
{
public:
    AccountClientEventHandler *accountClientEventHandler;
    ENetClient *accountClient;
};


ManaClient::ManaClient()
    : d(new ManaClientPrivate)
{
    d->accountClientEventHandler = new AccountClientEventHandler;
    d->accountClient = new ENetClient(d->accountClientEventHandler);
}

ManaClient::~ManaClient()
{
    delete d->accountClient;
    delete d->accountClientEventHandler;
    delete d;
}

void ManaClient::setAccountHandler(AccountHandlerInterface *handler)
{
    d->accountClientEventHandler->setAccountHandler(handler);
}

void ManaClient::handleNetworkTraffic()
{
    d->accountClient->service();
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

} // namespace Mana
