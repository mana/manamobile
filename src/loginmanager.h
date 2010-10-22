/*
 *  Mana Mobile
 *  Copyright (C) 2010, Thorbjørn Lindeijer
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
class CharacterListModel;

class LoginManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(CharacterListModel *characterListModel READ characterListModel
               CONSTANT)

public:
    explicit LoginManager(Mana::ManaClient *client, QObject *parent = 0);
    ~LoginManager();

    Q_INVOKABLE void connectToLoginServer(const QString &host,
                                          unsigned short port);

    Q_INVOKABLE void disconnectFromLoginServer();

    bool isConnected() const;

    Q_INVOKABLE void login(const QString &username, const QString &password);
    Q_INVOKABLE void chooseCharacter(int index);

    CharacterListModel *characterListModel() const
    { return mCharacterListModel; }

    QString error() const { return mError; }

signals:
    void connected();
    void disconnected();
    void isConnectedChanged();

    void loginSucceeded();
    void loginFailed();

    void errorChanged();

    void charactersChanged();

    void chooseCharacterSucceeded();
    void chooseCharacterFailed();

protected:
    void timerEvent(QTimerEvent *);

private:
    friend class AccountHandler;

    void onLoginFailed(int error);
    void onChooseCharacterSucceeded();
    void onChooseCharacterFailed(int error);

    QString mError;
    Mana::ManaClient *mClient;
    AccountHandler *mAccountHandler;
    int mNetworkTrafficTimer;

    QList<Mana::CharacterInfo> mCharacters;
    CharacterListModel *mCharacterListModel;
};

#endif // LOGINMANAGER_H
