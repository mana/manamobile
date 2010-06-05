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

#include "loginmanager.h"

#include <mana/accounthandlerinterface.h>
#include <mana/protocol.h>

#include <QTimerEvent>
#include <QDebug>

class AccountHandler : public Mana::AccountHandlerInterface
{
public:
    AccountHandler(LoginManager *loginManager)
        : lm(loginManager)
    {}

    void connected() { emit lm->connected(); }
    void disconnected() { emit lm->disconnected(); }

    void loginSucceeded() { emit lm->loginSucceeded(); }
    void loginFailed(int error) { lm->onLoginFailed(error); }

    void characterInfoReceived(const Mana::CharacterInfo &info)
    {
        lm->mCharacters.append(info);
        emit lm->charactersChanged();
    }

private:
    LoginManager *lm;
};


LoginManager::LoginManager(QObject *parent)
    : QObject(parent)
    , mClient(new Mana::ManaClient)
    , mAccountHandler(new AccountHandler(this))
    , mNetworkTrafficTimer(0)
{
    mClient->setAccountHandler(mAccountHandler);
}

LoginManager::~LoginManager()
{
    delete mClient;
    delete mAccountHandler;
}

void LoginManager::connectToLoginServer(const Mana::ServerAddress &server)
{
    qDebug() << Q_FUNC_INFO << server.host.c_str() << server.port;
    mClient->connectToAccountServer(server);

    if (!mNetworkTrafficTimer)
        mNetworkTrafficTimer = startTimer(100);
}

void LoginManager::disconnectFromLoginServer()
{
    mClient->disconnectFromAccountServer();
}

bool LoginManager::isConnected() const
{
    return mClient->isConnectedToAccountServer();
}

void LoginManager::login(const QString &username, const QString &password)
{
    qDebug() << Q_FUNC_INFO << username;
    mClient->login(username.toStdString(), password.toStdString());
}

void LoginManager::chooseCharacter(const Mana::CharacterInfo &character)
{
    qDebug() << Q_FUNC_INFO << character.name.c_str();
    mClient->chooseCharacter(character);
}

void LoginManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mNetworkTrafficTimer)
        mClient->handleNetworkTraffic();
}

void LoginManager::onLoginFailed(int error)
{
    qDebug() << Q_FUNC_INFO << error;

    switch (error) {
    case Mana::ERRMSG_FAILURE:
    default:
        mError = tr("Unknown error");
        break;
    case Mana::ERRMSG_INVALID_ARGUMENT:
        mError = tr("Wrong user name or password");
        break;
    case Mana::LOGIN_INVALID_TIME:
        mError = tr("Tried to login too fast");
        break;
    case Mana::LOGIN_INVALID_VERSION:
        mError = tr("Client version too old");
        break;
    case Mana::LOGIN_BANNED:
        mError = tr("Account is banned");
        break;
    }

    emit loginFailed();
}
