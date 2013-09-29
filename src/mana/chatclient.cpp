/*
 * Mana QML plugin
 * Copyright (C) 2010  Thorbjørn Lindeijer 
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

#include "chatclient.h"

#include <mana/protocol.h>

#include "messagein.h"
#include "messageout.h"

#include <iostream>

namespace Mana {

ChatClient::ChatClient(QObject *parent)
    : ENetClient(parent)
    , mAuthenticated(false)
{
}

void ChatClient::authenticate(const QString &token)
{
    // Send in the security token
    MessageOut msg(Protocol::PCMSG_CONNECT);
    msg.writeString(token, 32);
    send(msg);
}

void ChatClient::leave()
{
    send(MessageOut(Protocol::PCMSG_DISCONNECT));
}

void ChatClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case Protocol::CPMSG_CONNECT_RESPONSE:
        handleConnectResponse(message);
        break;
    case Protocol::CPMSG_DISCONNECT_RESPONSE:
        handleDisconnectResponse(message);
        break;
    case Protocol::XXMSG_INVALID:
        qWarning() << "(ChatClient::messageReceived) Invalid received! "
                      "Did we send an invalid message?";
        break;
    default:
        qDebug() << "(ChatClient::messageReceived) Unknown message "
                 << message;
        break;
    }
}

void ChatClient::handleConnectResponse(MessageIn &message)
{
    switch (message.readInt8()) {
    default:
        // Unknown error
        emit authenticationFailed(tr("Unknown error"));
        break;
    case ERRMSG_FAILURE:
        emit authenticationFailed(tr("Character not found"));
        break;
    case ERRMSG_OK:
        if (!mAuthenticated) {
            mAuthenticated = true;
            emit authenticatedChanged();
        }
        break;
    }
}

void ChatClient::handleDisconnectResponse(MessageIn &message)
{
    if (message.readInt8() != ERRMSG_OK)
        return;

    mAuthenticated = false;
    emit authenticatedChanged();

    disconnect();
}

} // namespace Mana
