/*
 *  Mana Mobile
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *
 *  This file is part of Mana Mobile.
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

#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "client.h"

#include <QObject>

class Client;
class MessageIn;

class CharacterInfo
{
public:
    QString name;
    int level;
    int money;
    int slot;
};

class LoginManager : public QObject
{
    Q_OBJECT

public:
    explicit LoginManager(QObject *parent = 0);

    void connectToLoginServer(const ServerAddress &server);
    void disconnectFromLoginServer();
    bool isConnected() const;

    void login(const QString &username, const QString &password);

    const QList<CharacterInfo> &characters() const
    { return mCharacters; }

    QString errorMessage() const { return mError; }

signals:
    void connected();
    void disconnected();

    void loginFailed();
    void loginSucceeded();

    void charactersChanged();

private slots:
    void handleMessage(const QByteArray &message);

private:
    void handleLoginResponse(MessageIn &message);
    void handleCharacterInfo(MessageIn &message);

    QString mError;
    Client *mClient;

    QString mUpdateHost;
    QList<CharacterInfo> mCharacters;
};

#endif // LOGINMANAGER_H
