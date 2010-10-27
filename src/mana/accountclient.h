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

class CharacterListModel;

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

    Q_PROPERTY(bool registrationAllowed READ registrationAllowed NOTIFY registrationAllowedChanged)
    Q_PROPERTY(int minimumNameLength READ minimumNameLength NOTIFY minimumNameLengthChanged)
    Q_PROPERTY(int maximumNameLength READ maximumNameLength NOTIFY maximumNameLengthChanged)
    Q_PROPERTY(QString captchaUrl READ captchaUrl NOTIFY captchaUrlChanged)
    Q_PROPERTY(QString captchaInstructions READ captchaInstructions NOTIFY captchaInstructionsChanged)

    Q_PROPERTY(QString updateHost READ updateHost NOTIFY updateHostChanged)
    Q_PROPERTY(QString dataUrl READ dataUrl NOTIFY dataUrlChanged)
    Q_PROPERTY(QString token READ token NOTIFY tokenChanged)
    Q_PROPERTY(QString chatServerHost READ chatServerHost NOTIFY chatServerHostChanged)
    Q_PROPERTY(quint16 chatServerPort READ chatServerPort NOTIFY chatServerPortChanged)
    Q_PROPERTY(QString gameServerHost READ gameServerHost NOTIFY gameServerHostChanged)
    Q_PROPERTY(quint16 gameServerPort READ gameServerPort NOTIFY gameServerPortChanged)

    Q_PROPERTY(Mana::CharacterListModel *characterListModel READ characterListModel CONSTANT)

public:
    AccountClient(QObject *parent = 0);

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

    CharacterListModel *characterListModel() const
    { return mCharacterListModel; }

    Q_INVOKABLE void requestRegistrationInfo();
    Q_INVOKABLE void registerAccount(const QString &username,
                                     const QString &password,
                                     const QString &email,
                                     const QString &captchaResponse);
    Q_INVOKABLE void unregisterAccount(const QString &username,
                                       const QString &password);
    Q_INVOKABLE void login(const QString &username, const QString &password);
    Q_INVOKABLE void createCharacter(const QString &name,
                                     bool gender,
                                     int hairStyle,
                                     int hairColor,
                                     const QList<int> &stats);
    Q_INVOKABLE void deleteCharacter(int index);
    Q_INVOKABLE void chooseCharacter(int index);

signals:
    void registrationSucceeded();
    void registrationFailed(int error, const QString &errorMessage);

    void unregisterSucceeded();
    void unregisterFailed(int error, const QString &errorMessage);

    void loginSucceeded();
    void loginFailed(int error, const QString &errorMessage);

    void createCharacterSucceeded();
    void createCharacterFailed(int error, const QString &errorMessage);

    void deleteCharacterSucceeded();
    void deleteCharacterFailed(int error, const QString &errorMessage);

    void characterInfoReceived(const Mana::CharacterInfo &info);

    void chooseCharacterSucceeded();
    void chooseCharacterFailed(int error, const QString &errorMessage);

    void registrationAllowedChanged();
    void minimumNameLengthChanged();
    void maximumNameLengthChanged();
    void captchaUrlChanged();
    void captchaInstructionsChanged();

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
    void readUpdateHost(MessageIn &message);

    void handleRegistrationInfo(MessageIn &message);
    void handleRegisterResponse(MessageIn &message);
    void handleUnregisterResponse(MessageIn &message);
    void handleLoginResponse(MessageIn &message);
    void handleCharacterCreateResponse(MessageIn &message);
    void handleCharacterDeleteResponse(MessageIn &message);
    void handleCharacterInfo(MessageIn &message);
    void handleCharacterSelectResponse(MessageIn &message);

    static QString standardErrorMessage(int error);
    static QString registrationErrorMessage(int error);
    static QString loginErrorMessage(int error);
    static QString createCharacterErrorMessage(int error);
    static QString chooseCharacterErrorMessage(int error);

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

    QList<Mana::CharacterInfo> mCharacters;

    CharacterListModel *mCharacterListModel;

    int mDeleteIndex;
};

} // namespace Mana

#endif // ACCOUNTCLIENT_H
