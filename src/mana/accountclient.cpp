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

#include "accountclient.h"

#include "characterlistmodel.h"
#include "messagein.h"
#include "messageout.h"
#include "protocol.h"
#include "sha256.h"

#include <safeassert.h>

#include <iostream>

namespace Mana {

AccountClient::AccountClient(QObject *parent)
    : ENetClient(parent)
    , mRegistrationAllowed(false)
    , mMinimumNameLength(2)
    , mMaximumNameLength(16)
    , mGameServerPort(0)
    , mChatServerPort(0)
    , mCharacterListModel(new CharacterListModel(this))
    , mDeleteIndex(-1)
{}

void AccountClient::requestRegistrationInfo()
{
    send(MessageOut(PAMSG_REQUEST_REGISTER_INFO));
}

static QByteArray passwordHash(const QString &username,
                               const QString &password)
{
    QByteArray combination;
    combination += username.toUtf8();
    combination += password.toUtf8();
    return sha256(combination);
}

void AccountClient::registerAccount(const QString &username,
                                    const QString &password,
                                    const QString &email,
                                    const QString &captchaResponse)
{
    MessageOut registerMessage(PAMSG_REGISTER);
    registerMessage.writeInt32(0); // client version
    registerMessage.writeString(username);
    registerMessage.writeString(passwordHash(username, password));
    registerMessage.writeString(email);
    registerMessage.writeString(captchaResponse);
    send(registerMessage);
}

void AccountClient::unregisterAccount(const QString &username,
                                      const QString &password)
{
    MessageOut unregisterMessage(PAMSG_UNREGISTER);
    unregisterMessage.writeString(username);
    unregisterMessage.writeString(passwordHash(username, password));
    send(unregisterMessage);
}

void AccountClient::login(const QString &username,
                          const QString &password)
{
    MessageOut loginMessage(PAMSG_LOGIN);
    loginMessage.writeInt32(0); // client version
    loginMessage.writeString(username);
    loginMessage.writeString(passwordHash(username, password));
    send(loginMessage);
}

void AccountClient::createCharacter(const QString &name,
                                    bool gender,
                                    int hairStyle, int hairColor,
                                    const QList<int> &stats)
{
    MessageOut createMessage(PAMSG_CHAR_CREATE);
    createMessage.writeString(name);
    createMessage.writeInt8(hairStyle);
    createMessage.writeInt8(hairColor);
    createMessage.writeInt8(gender);
    foreach (int stat, stats)
        createMessage.writeInt16(stat);
    send(createMessage);
}

void AccountClient::deleteCharacter(int index)
{
    SAFE_ASSERT(index >= 0 && index < mCharacters.size(), return);
    const CharacterInfo &character = mCharacters.at(index);

    mDeleteIndex = index;

    MessageOut m(PAMSG_CHAR_DELETE);
    m.writeInt8(character.slot);
    send(m);
}

void AccountClient::chooseCharacter(int index)
{
    SAFE_ASSERT(index >= 0 && index < mCharacters.size(), return);
    const CharacterInfo &character = mCharacters.at(index);

    MessageOut m(PAMSG_CHAR_SELECT);
    m.writeInt8(character.slot);
    send(m);
}

void AccountClient::changeEmail(const QString &email)
{
    MessageOut msg(PAMSG_EMAIL_CHANGE);
    msg.writeString(email);
    send(msg);
}

void AccountClient::changePassword(const QString &username,
                                   const QString &oldPassword,
                                   const QString &newPassword)
{
    MessageOut msg(PAMSG_PASSWORD_CHANGE);
    msg.writeString(passwordHash(username, oldPassword));
    msg.writeString(passwordHash(username, newPassword));
    send(msg);
}

void AccountClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case APMSG_REGISTER_INFO_RESPONSE:
        handleRegistrationInfo(message);
        break;
    case APMSG_REGISTER_RESPONSE:
        handleRegisterResponse(message);
        break;
    case APMSG_UNREGISTER_RESPONSE:
        handleUnregisterResponse(message);
        break;
    case APMSG_LOGIN_RESPONSE:
        handleLoginResponse(message);
        break;
    case APMSG_CHAR_CREATE_RESPONSE:
        handleCharacterCreateResponse(message);
        break;
    case APMSG_CHAR_DELETE_RESPONSE:
        handleCharacterDeleteResponse(message);
        break;
    case APMSG_CHAR_INFO:
        handleCharacterInfo(message);
        break;
    case APMSG_CHAR_SELECT_RESPONSE:
        handleCharacterSelectResponse(message);
        break;
    case APMSG_EMAIL_CHANGE_RESPONSE:
        handleEmailChangeResponse(message);
        break;
    case APMSG_PASSWORD_CHANGE_RESPONSE:
        handlePasswordChangeResponse(message);
        break;
    case XXMSG_INVALID:
        qWarning() << "(AccountClient::messageReceived) Invalid received! "
                "Did we send an invalid message?";
        break;
    default:
        qDebug() << "(AccountClient::messageReceived) Unknown message "
                << message;
        break;
    }
}

void AccountClient::readUpdateHost(MessageIn &message)
{
    mUpdateHost = message.readString();
    emit updateHostChanged();

    mDataUrl = message.readString();
    if (!mDataUrl.isEmpty()) {
        if (!mDataUrl.endsWith(QLatin1Char('/')))
            mDataUrl += QLatin1Char('/');
        emit dataUrlChanged();
    }
}

void AccountClient::handleRegistrationInfo(MessageIn &message)
{
    mRegistrationAllowed = message.readInt8();
    mMinimumNameLength = message.readInt8();
    mMaximumNameLength = message.readInt8();
    mCaptchaUrl = message.readString();
    mCaptchaInstructions = message.readString();

    emit registrationAllowedChanged();
    emit minimumNameLengthChanged();
    emit maximumNameLengthChanged();
    emit captchaUrlChanged();
    emit captchaInstructionsChanged();
}

void AccountClient::handleRegisterResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK) {
        readUpdateHost(message);
        emit registrationSucceeded();
    } else {
        emit registrationFailed(error, registrationErrorMessage(error));
    }
}

void AccountClient::handleUnregisterResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK)
        emit unregisterSucceeded();
    else
        // Unregister error messages are same as login error messages
        emit unregisterFailed(error, loginErrorMessage(error));
}

void AccountClient::handleLoginResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK) {
        readUpdateHost(message);
        emit loginSucceeded();
    } else {
        emit loginFailed(error, loginErrorMessage(error));
    }
}

void AccountClient::handleCharacterCreateResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK)
        emit createCharacterSucceeded();
    else
        emit createCharacterFailed(error, createCharacterErrorMessage(error));
}

void AccountClient::handleCharacterDeleteResponse(MessageIn &message)
{
    SAFE_ASSERT(mDeleteIndex >= 0 && mDeleteIndex <= mCharacters.size(),
                return);

    const int error = message.readInt8();

    if (error == ERRMSG_OK) {
        mCharacters.removeAt(mDeleteIndex);
        mCharacterListModel->setCharacters(mCharacters);
        emit deleteCharacterSucceeded();
    } else {
        // Delete error messages are same as choose character error messages
        emit deleteCharacterFailed(error, chooseCharacterErrorMessage(error));
    }

    mDeleteIndex = -1;
}

void AccountClient::handleCharacterInfo(MessageIn &message)
{
    CharacterInfo info;

    info.slot = message.readInt8();
    info.name = message.readString();
    message.readInt8(); // gender
    message.readInt8(); // hair style
    message.readInt8(); // hair color
    info.level = message.readInt16();
    message.readInt16(); // character points
    message.readInt16(); // correction points
    info.money = message.readInt32();
    for (int i = 0; i < 7; i++)
    {
        message.readInt8(); // attribute
    }

    mCharacters.append(info);
    mCharacterListModel->setCharacters(mCharacters);

    emit characterInfoReceived(info);
}

void AccountClient::handleCharacterSelectResponse(MessageIn &message)
{
    const int error = message.readInt8();
    if (error == ERRMSG_OK) {
        // Remember chat and game server hosts
        mToken = message.readString(32);
        mGameServerHost = message.readString();
        mGameServerPort = message.readInt16();
        mChatServerHost = message.readString();
        mChatServerPort = message.readInt16();

        emit tokenChanged();
        emit gameServerHostChanged();
        emit gameServerPortChanged();
        emit chatServerHostChanged();
        emit chatServerPortChanged();

        qDebug() << "Game server: " << mGameServerHost << ":"
                << mGameServerPort;
        qDebug() << "Chat server: " << mChatServerHost << ":"
                << mChatServerPort;

        emit chooseCharacterSucceeded();
    } else {
        emit chooseCharacterFailed(error, chooseCharacterErrorMessage(error));
    }
}

void AccountClient::handleEmailChangeResponse(MessageIn &message)
{
    const int error = message.readInt8();
    if (error == ERRMSG_OK)
        emit emailChangeSucceeded();
    else
        emit emailChangeFailed(error, emailChangeErrorMessage(error));
}

void AccountClient::handlePasswordChangeResponse(MessageIn &message)
{
    const int error = message.readInt8();
    if (error == ERRMSG_OK)
        emit passwordChangeSucceeded();
    else
        emit passwordChangeFailed(error, passwordChangeErrorMessage(error));
}

QString AccountClient::standardErrorMessage(int error)
{
    switch (error) {
    case ERRMSG_NO_LOGIN:
        return tr("You don't seem to be logged in, please try again");
    default:
        return tr("Unknown error");
    }
}

QString AccountClient::registrationErrorMessage(int error)
{
    switch (error) {
    case REGISTER_INVALID_VERSION:
        return tr("Client version is too old");
    case ERRMSG_INVALID_ARGUMENT:
        return tr("Wrong username, password or email address");
    case REGISTER_EXISTS_USERNAME:
        return tr("Username already exists");
    case REGISTER_EXISTS_EMAIL:
        return tr("Email address already exists");
    case REGISTER_CAPTCHA_WRONG:
        return tr("You took too long with the captcha or your "
                  "response was incorrect");
    default:
        return standardErrorMessage(error);
    }
}

QString AccountClient::loginErrorMessage(int error)
{
    switch (error) {
    case ERRMSG_INVALID_ARGUMENT:
        return tr("Wrong user name or password");
    case LOGIN_INVALID_TIME:
        return tr("Tried to login too fast");
    case LOGIN_INVALID_VERSION:
        return tr("Client version too old");
    case LOGIN_BANNED:
        return tr("Account is banned");
    default:
        return standardErrorMessage(error);
    }
}

QString AccountClient::createCharacterErrorMessage(int error)
{
    switch (error) {
    case CREATE_TOO_MUCH_CHARACTERS:
        return tr("No empty slot");
    case ERRMSG_INVALID_ARGUMENT:
        return tr("Invalid name");
    case CREATE_EXISTS_NAME:
        return tr("Character's name already exists");
    case CREATE_INVALID_HAIRSTYLE:
        return tr("Invalid hairstyle");
    case CREATE_INVALID_HAIRCOLOR:
        return tr("Invalid hair color");
    case CREATE_INVALID_GENDER:
        return tr("Invalid gender");
    case CREATE_ATTRIBUTES_TOO_HIGH:
        return tr("Character's stats are too high");
    case CREATE_ATTRIBUTES_TOO_LOW:
        return tr("Character's stats are too low");
    case CREATE_ATTRIBUTES_OUT_OF_RANGE:
        return tr("At least one stat is out of the permitted range");
    default:
        return standardErrorMessage(error);
    }
}

QString AccountClient::chooseCharacterErrorMessage(int error)
{
    switch (error) {
    case ERRMSG_INVALID_ARGUMENT:
        return tr("No such character");
    case ERRMSG_FAILURE:
        return tr("No game server found for the map the character is on");
    default:
        return standardErrorMessage(error);
    }
}

QString AccountClient::emailChangeErrorMessage(int error)
{
    switch (error) {
    case ERRMSG_INVALID_ARGUMENT:
        return tr("New email address incorrect");
    case ERRMSG_FAILURE:
        return tr("Old email address incorrect");
    case ERRMSG_EMAIL_ALREADY_EXISTS:
        return tr("The new email address already exists");
    default:
        return standardErrorMessage(error);
    }
}

QString AccountClient::passwordChangeErrorMessage(int error)
{
    switch (error) {
    case ERRMSG_INVALID_ARGUMENT:
        return tr("New password incorrect");
    case ERRMSG_FAILURE:
        return tr("Old password incorrect");
    default:
        return standardErrorMessage(error);
    }
}

} // namespace Mana
