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

#ifndef ACCOUNTCLIENT_H
#define ACCOUNTCLIENT_H

#include "character.h"
#include "enetclient.h"

#include <QMap>

namespace Mana {

class CharacterListModel;

/**
 * The account client allows interacting with the account server.
 */
class AccountClient : public ENetClient
{
    Q_OBJECT

    Q_PROPERTY(bool registrationAllowed READ registrationAllowed NOTIFY registrationInfoChanged)
    Q_PROPERTY(int minimumNameLength READ minimumNameLength NOTIFY registrationInfoChanged)
    Q_PROPERTY(int maximumNameLength READ maximumNameLength NOTIFY registrationInfoChanged)
    Q_PROPERTY(QString captchaUrl READ captchaUrl NOTIFY registrationInfoChanged)
    Q_PROPERTY(QString captchaInstructions READ captchaInstructions NOTIFY registrationInfoChanged)

    Q_PROPERTY(QString updateHost READ updateHost NOTIFY serverInfoChanged)
    Q_PROPERTY(QString dataUrl READ dataUrl NOTIFY serverInfoChanged)
    Q_PROPERTY(QString token READ token NOTIFY tokenChanged)
    Q_PROPERTY(QString chatServerHost READ chatServerHost NOTIFY chatServerChanged)
    Q_PROPERTY(quint16 chatServerPort READ chatServerPort NOTIFY chatServerChanged)
    Q_PROPERTY(QString gameServerHost READ gameServerHost NOTIFY gameServerChanged)
    Q_PROPERTY(quint16 gameServerPort READ gameServerPort NOTIFY gameServerChanged)

    Q_PROPERTY(QString username READ username NOTIFY usernameChanged)
    Q_PROPERTY(QString playerName READ playerName NOTIFY playerNameChanged)


    Q_PROPERTY(int maxCharacters READ maxCharacters NOTIFY serverInfoChanged)
    Q_PROPERTY(int numberOfCharacters READ numberOfCharacters NOTIFY characterInfoReceived)
    Q_PROPERTY(Mana::CharacterListModel *characterListModel READ characterListModel CONSTANT)

public:
    AccountClient(QObject *parent = 0);
    ~AccountClient();

    bool registrationAllowed() const { return mRegistrationAllowed; }
    int minimumNameLength() const { return mMinimumNameLength; }
    int maximumNameLength() const { return mMaximumNameLength; }
    QString captchaUrl() const { return mCaptchaUrl; }
    QString captchaInstructions() const { return mCaptchaInstructions; }

    QString updateHost() const { return mUpdateHost; }
    QString dataUrl() const { return mDataUrl; }
    QString token() const { return mToken; }
    QString gameServerHost() const { return mGameServerHost; }
    quint16 gameServerPort() const { return mGameServerPort; }
    QString chatServerHost() const { return mChatServerHost; }
    quint16 chatServerPort() const { return mChatServerPort; }

    QString username() const { return mUsername; }
    QString playerName() const { return mPlayerName; }

    int maxCharacters() const { return mMaxCharacters; }
    int numberOfCharacters() const;

    CharacterListModel *characterListModel() const
    { return mCharacterListModel; }

    Q_INVOKABLE void requestRegistrationInfo();
    Q_INVOKABLE void registerAccount(const QString &username,
                                     const QString &password,
                                     const QString &email,
                                     const QString &captchaResponse);
    Q_INVOKABLE void unregisterAccount(const QString &username,
                                       const QString &password);
    Q_INVOKABLE void login(const QString &username,
                           const QString &password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void reconnect(const QString &token);

    Q_INVOKABLE void createCharacter(const QString &name,
                                     int gender,
                                     int hairStyle,
                                     int hairColor,
                                     const QList<int> &stats);
    Q_INVOKABLE void deleteCharacter(int index);
    Q_INVOKABLE void chooseCharacter(int index);
    Q_INVOKABLE void changeEmail(const QString &email);
    Q_INVOKABLE void changePassword(const QString &oldPassword,
                                    const QString &newPassword);

signals:
    void registrationSucceeded();
    void registrationFailed(int error, const QString &errorMessage);

    void unregisterSucceeded();
    void unregisterFailed(int error, const QString &errorMessage);

    void loginSucceeded();
    void loginFailed(int error, const QString &errorMessage);

    void reconnectSucceeded();
    void reconnectFailed();

    void loggedOut();

    void createCharacterSucceeded();
    void createCharacterFailed(int error, const QString &errorMessage);

    void deleteCharacterSucceeded();
    void deleteCharacterFailed(int error, const QString &errorMessage);

    void characterInfoReceived(const Character *info);
    void dataUrlReceived();

    void chooseCharacterSucceeded();
    void chooseCharacterFailed(int error, const QString &errorMessage);

    void emailChangeSucceeded();
    void emailChangeFailed(int error, const QString &errorMessage);

    void passwordChangeSucceeded();
    void passwordChangeFailed(int error, const QString &errorMessage);

    void registrationInfoChanged();

    void characterDataReceived(int count);

    void serverInfoChanged();
    void tokenChanged();
    void chatServerChanged();
    void gameServerChanged();

    void usernameChanged();
    void playerNameChanged();

protected:
    void messageReceived(MessageIn &message);

private:
    void readServerInfo(MessageIn &message);
    void readCharacterInfo(MessageIn &message);

    void login(const QString &username,
               const QString &password,
               const QByteArray &salt);

    void handleRegistrationInfo(MessageIn &message);
    void handleRegisterResponse(MessageIn &message);
    void handleUnregisterResponse(MessageIn &message);
    void handleLoginResponse(MessageIn &message);
    void handleLogoutResponse(MessageIn &message);
    void handleCharacterCreateResponse(MessageIn &message);
    void handleCharacterDeleteResponse(MessageIn &message);
    void handleCharacterInfo(MessageIn &message);
    void handleCharacterSelectResponse(MessageIn &message);
    void handleEmailChangeResponse(MessageIn &message);
    void handlePasswordChangeResponse(MessageIn &message);
    void handleReconnectResponse(MessageIn &message);
    void handleSaltResponse(MessageIn &message);

    static QString standardErrorMessage(int error);
    static QString registrationErrorMessage(int error);
    static QString loginErrorMessage(int error);
    static QString createCharacterErrorMessage(int error);
    static QString chooseCharacterErrorMessage(int error);
    static QString emailChangeErrorMessage(int error);
    static QString passwordChangeErrorMessage(int error);

    bool mRegistrationAllowed;
    int mMinimumNameLength;
    int mMaximumNameLength;
    QString mCaptchaUrl;
    QString mCaptchaInstructions;

    QString mUpdateHost;
    QString mDataUrl;
    QString mToken;
    QString mGameServerHost;
    QString mChatServerHost;
    quint16 mGameServerPort;
    quint16 mChatServerPort;

    QString mUsername;
    QString mPlayerName;

    int mMaxCharacters;
    QList<Character *> mCharacters;

    CharacterListModel *mCharacterListModel;

    int mDeleteIndex;
    QString mPendingUsername;
    QString mPendingPassword;
    QString mPendingPlayerName;
};

} // namespace Mana

#endif // ACCOUNTCLIENT_H
