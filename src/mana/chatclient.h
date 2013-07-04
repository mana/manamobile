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

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "enetclient.h"

namespace Mana {

class ManaClient;

/**
 * The chat client allows interacting with the chat server.
 */
class ChatClient : public ENetClient
{
    Q_OBJECT

    Q_PROPERTY(bool authenticated READ authenticated NOTIFY authenticatedChanged)

public:
    ChatClient(QObject *parent = 0);

    bool authenticated() const { return mAuthenticated; }

    Q_INVOKABLE void authenticate(const QString &token);

signals:
    void authenticationFailed(const QString &errorMessage);

    void authenticatedChanged();

protected:
    void messageReceived(MessageIn &message);

private:
    void handleAuthenticationResponse(MessageIn &message);

    bool mAuthenticated;
};

} // namespace Mana

#endif // CHATCLIENT_H
