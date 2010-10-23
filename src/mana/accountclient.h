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

class CharacterInfo
{
public:
    QString name;
    int level;
    int money;
    int slot;
};

/**
 * The account client allows interacting with the account server.
 */
class AccountClient : public ENetClient
{
    Q_OBJECT

    Q_PROPERTY(QString updateHost READ updateHost NOTIFY updateHostChanged)
    Q_PROPERTY(QString dataUrl READ dataUrl NOTIFY dataUrlChanged)
    Q_PROPERTY(QString token READ token NOTIFY tokenChanged)
    Q_PROPERTY(QString chatServerHost READ chatServerHost NOTIFY chatServerHostChanged)
    Q_PROPERTY(quint16 chatServerPort READ chatServerPort NOTIFY chatServerPortChanged)
    Q_PROPERTY(QString gameServerHost READ gameServerHost NOTIFY gameServerHostChanged)
    Q_PROPERTY(quint16 gameServerPort READ gameServerPort NOTIFY gameServerPortChanged)

public:
    AccountClient(QObject *parent = 0)
        : ENetClient(parent)
        , mGameServerPort(0)
        , mChatServerPort(0)
    {}

    QString updateHost() const { return mUpdateHost; }
    QString dataUrl() const { return mDataUrl; }
    QString token() const { return mToken; }
    QString gameServerHost() const { return mGameServerHost; }
    quint16 gameServerPort() const { return mGameServerPort; }
    QString chatServerHost() const { return mChatServerHost; }
    quint16 chatServerPort() const { return mChatServerPort; }

    Q_INVOKABLE void login(const QString &username, const QString &password);
    void chooseCharacter(const CharacterInfo &character);

signals:
    void loginSucceeded();
    void loginFailed(int error);

    void characterInfoReceived(const Mana::CharacterInfo &info);

    void chooseCharacterSucceeded();
    void chooseCharacterFailed(int error);

    void updateHostChanged();
    void dataUrlChanged();
    void tokenChanged();
    void chatServerHostChanged();
    void chatServerPortChanged();
    void gameServerHostChanged();
    void gameServerPortChanged();

protected:
    void messageReceived(MessageIn &message);

private:
    void handleLoginResponse(MessageIn &message);
    void handleCharacterInfo(MessageIn &message);
    void handleCharacterSelectResponse(MessageIn &message);

    QString mUpdateHost;
    QString mDataUrl;
    QString mToken;
    QString mGameServerHost;
    QString mChatServerHost;
    quint16 mGameServerPort;
    quint16 mChatServerPort;
};

} // namespace Mana

#endif // ACCOUNTCLIENT_H
