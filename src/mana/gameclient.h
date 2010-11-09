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

#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "enetclient.h"

namespace Mana {

class Being;
class BeingListModel;

/**
 * The game client allows interacting with the game server.
 *
 * The player name should be set so that the game server can identify who the
 * current player is.
 */
class GameClient : public ENetClient
{
    Q_OBJECT

    Q_PROPERTY(bool authenticated READ authenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(QString currentMap READ currentMap NOTIFY mapChanged)
    Q_PROPERTY(Mana::BeingListModel *beingListModel READ beingListModel CONSTANT)
    Q_PROPERTY(Mana::Being *player READ player NOTIFY playerChanged)

    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)

public:
    GameClient(QObject *parent = 0);
    ~GameClient();

    bool authenticated() const { return mAuthenticated; }
    QString currentMap() const { return mCurrentMap; }
    BeingListModel *beingListModel() const;
    Being *player() const;

    QString playerName() const;
    void setPlayerName(const QString &name);

    Q_INVOKABLE void authenticate(const QString &token);
    Q_INVOKABLE void walkTo(int x, int y);
    Q_INVOKABLE void say(const QString &text);

signals:
    void authenticationFailed(const QString &errorMessage);

    void authenticatedChanged();
    void mapChanged(const QString &name, int x, int y);
    void playerChanged();

    void playerNameChanged();

protected:
    void messageReceived(MessageIn &message);

private:
    void handleAuthenticationResponse(MessageIn &message);
    void handlePlayerMapChanged(MessageIn &message);

    bool mAuthenticated;
    QString mCurrentMap;

    BeingListModel *mBeingListModel;
};

} // namespace Mana

#endif // GAMECLIENT_H
