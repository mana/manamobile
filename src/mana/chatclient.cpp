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

#include "chatclient.h"

#include <mana/protocol.h>

#include "messagein.h"
#include "messageout.h"

#include <iostream>

namespace Mana {

ChatClient::ChatClient(QObject *parent)
    : ENetClient(parent)
{
}

void ChatClient::sendToken(const QString &token)
{
    // Send in the security token
    MessageOut msg(PCMSG_CONNECT);
    msg.writeString(token, 32);
    send(msg);
}

void ChatClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case XXMSG_INVALID:
        std::cerr << "(ChatClient::messageReceived) Invalid received! "
                "Did we send an invalid message?" << std::endl;
        break;
    default:
        std::cout << "(ChatClient::messageReceived) Unknown message "
                << message << std::endl;
        break;
    }
}

} // namespace Mana
