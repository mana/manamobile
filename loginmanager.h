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

#include <mana/accounthandlerinterface.h>
#include <mana/manaclient.h>

#include <QObject>

class AccountHandler;

class LoginManager : public QObject
{
    Q_OBJECT

public:
    explicit LoginManager(QObject *parent = 0);
    ~LoginManager();

    void connectToLoginServer(const Mana::ServerAddress &server);
    void disconnectFromLoginServer();
    bool isConnected() const;

    void login(const QString &username, const QString &password);

    void chooseCharacter(const Mana::CharacterInfo &character);

    const QList<Mana::CharacterInfo> &characters() const
    { return mCharacters; }

    QString errorMessage() const { return mError; }

signals:
    void connected();
    void disconnected();

    void loginFailed();
    void loginSucceeded();

    void charactersChanged();

protected:
    void timerEvent(QTimerEvent *);

private:
    friend class AccountHandler;

    void onLoginFailed(int error);

    QString mError;
    Mana::ManaClient *mClient;
    AccountHandler *mAccountHandler;
    int mNetworkTrafficTimer;

    QString mUpdateHost;
    QList<Mana::CharacterInfo> mCharacters;
};

#endif // LOGINMANAGER_H
