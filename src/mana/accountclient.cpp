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

#include "accountclient.h"

#include "attributelistmodel.h"
#include "characterlistmodel.h"
#include "messagein.h"
#include "messageout.h"
#include "protocol.h"

#include <safeassert.h>

#include <QCryptographicHash>

static inline QByteArray sha256(const QByteArray &data)
{
    return QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex();
}

namespace Mana {

AccountClient::AccountClient(QObject *parent)
    : ENetClient(parent)
    , mRegistrationAllowed(false)
    , mMinimumNameLength(2)
    , mMaximumNameLength(16)
    , mGameServerPort(0)
    , mChatServerPort(0)
    , mMaxCharacters(0)
    , mCharacterListModel(new CharacterListModel(this))
    , mDeleteIndex(-1)
{}

AccountClient::~AccountClient()
{
    qDeleteAll(mCharacters);
}

void AccountClient::requestRegistrationInfo()
{
    send(MessageOut(Protocol::PAMSG_REQUEST_REGISTER_INFO));
}

static QByteArray passwordHash(const QString &username,
                               const QString &password)
{
    QByteArray combination;
    combination += username.toUtf8();
    combination += password.toUtf8();
    return sha256(combination);
}

static QByteArray passwordSaltedHash(const QString &username,
                                     const QString &password,
                                     const QByteArray &salt)
{
    QByteArray combination;
    combination += username.toUtf8();
    combination += password.toUtf8();
    return sha256(sha256(sha256(combination)).append(salt));
}

void AccountClient::registerAccount(const QString &username,
                                    const QString &password,
                                    const QString &email,
                                    const QString &captchaResponse)
{
    MessageOut registerMessage(Protocol::PAMSG_REGISTER);
    registerMessage.writeInt32(PROTOCOL_VERSION);
    registerMessage.writeString(username);
    registerMessage.writeString(passwordHash(username, password));
    registerMessage.writeString(email);
    registerMessage.writeString(captchaResponse);
    send(registerMessage);

    mPendingUsername = username;
}

void AccountClient::unregisterAccount(const QString &username,
                                      const QString &password)
{
    MessageOut unregisterMessage(Protocol::PAMSG_UNREGISTER);
    unregisterMessage.writeString(username);
    unregisterMessage.writeString(passwordHash(username, password));
    send(unregisterMessage);
}

void AccountClient::login(const QString &username,
                          const QString &password)
{
    MessageOut saltMessage(Protocol::PAMSG_LOGIN_RNDTRGR);
    saltMessage.writeString(username);

    send(saltMessage);

    mPendingUsername = username;
    mPendingPassword = password;
}

void AccountClient::login(const QString &username,
                          const QString &password,
                          const QByteArray &salt)
{
    MessageOut loginMessage(Protocol::PAMSG_LOGIN);
    loginMessage.writeInt32(PROTOCOL_VERSION);
    loginMessage.writeString(username);
    loginMessage.writeString(passwordSaltedHash(username, password, salt));
    send(loginMessage);
}

static bool slotAvailable(const QList<Character *> &chars, int slot)
{
    for (int i = 0; i < chars.size(); ++i)
        if (chars.at(i)->characterSlot() == slot)
            return false;

    return true;
}

static int nextFreeSlot(const QList<Character *> &chars, int maxChars)
{
    for (int slot = 1; slot <= maxChars; ++slot)
        if (slotAvailable(chars, slot))
            return slot;

    return -1;
}

void AccountClient::createCharacter(const QString &name,
                                    int gender,
                                    int hairStyle, int hairColor,
                                    const QList<int> &stats)
{
    const int slot = nextFreeSlot(mCharacters, mMaxCharacters);
    SAFE_ASSERT(slot != -1, return)

    MessageOut createMessage(Protocol::PAMSG_CHAR_CREATE);
    createMessage.writeString(name);
    createMessage.writeInt8(hairStyle);
    createMessage.writeInt8(hairColor);
    createMessage.writeInt8(gender);
    createMessage.writeInt8(slot);
    foreach (int stat, stats)
        createMessage.writeInt16(stat);
    send(createMessage);
}

void AccountClient::deleteCharacter(int index)
{
    SAFE_ASSERT(index >= 0 && index < mCharacters.size(), return);
    const Character *character = mCharacters.at(index);

    mDeleteIndex = index;

    MessageOut m(Protocol::PAMSG_CHAR_DELETE);
    m.writeInt8(character->characterSlot());
    send(m);
}

void AccountClient::chooseCharacter(int index)
{
    SAFE_ASSERT(index >= 0 && index < mCharacters.size(), return);
    const Character *character = mCharacters.at(index);

    MessageOut m(Protocol::PAMSG_CHAR_SELECT);
    m.writeInt8(character->characterSlot());
    send(m);

    mPendingPlayerName = character->name();
}

void AccountClient::changeEmail(const QString &email)
{
    MessageOut msg(Protocol::PAMSG_EMAIL_CHANGE);
    msg.writeString(email);
    send(msg);
}

void AccountClient::changePassword(const QString &oldPassword,
                                   const QString &newPassword)
{
    MessageOut msg(Protocol::PAMSG_PASSWORD_CHANGE);
    msg.writeString(passwordHash(mUsername, oldPassword));
    msg.writeString(passwordHash(mUsername, newPassword));
    send(msg);
}

void AccountClient::messageReceived(MessageIn &message)
{
    switch (message.id()) {
    case Protocol::APMSG_REGISTER_INFO_RESPONSE:
        handleRegistrationInfo(message);
        break;
    case Protocol::APMSG_REGISTER_RESPONSE:
        handleRegisterResponse(message);
        break;
    case Protocol::APMSG_UNREGISTER_RESPONSE:
        handleUnregisterResponse(message);
        break;
    case Protocol::APMSG_LOGIN_RESPONSE:
        handleLoginResponse(message);
        break;
    case Protocol::APMSG_LOGIN_RNDTRGR_RESPONSE:
        handleSaltResponse(message);
        break;
    case Protocol::APMSG_CHAR_CREATE_RESPONSE:
        handleCharacterCreateResponse(message);
        break;
    case Protocol::APMSG_CHAR_DELETE_RESPONSE:
        handleCharacterDeleteResponse(message);
        break;
    case Protocol::APMSG_CHAR_INFO:
        handleCharacterInfo(message);
        break;
    case Protocol::APMSG_CHAR_SELECT_RESPONSE:
        handleCharacterSelectResponse(message);
        break;
    case Protocol::APMSG_EMAIL_CHANGE_RESPONSE:
        handleEmailChangeResponse(message);
        break;
    case Protocol::APMSG_PASSWORD_CHANGE_RESPONSE:
        handlePasswordChangeResponse(message);
        break;
    case Protocol::XXMSG_INVALID:
        qWarning() << "(AccountClient::messageReceived) Invalid received! "
                "Did we send an invalid message?";
        break;
    default:
        qDebug() << "(AccountClient::messageReceived) Unknown message "
                << message;
        break;
    }
}

void AccountClient::readServerInfo(MessageIn &message)
{
    mUpdateHost = message.readString();
    mDataUrl = message.readString();
    mMaxCharacters = message.readInt8();

    if (!mDataUrl.isEmpty()) {
        if (!mDataUrl.endsWith(QLatin1Char('/')))
            mDataUrl += QLatin1Char('/');
    }

    emit serverInfoChanged();
}

void AccountClient::handleRegistrationInfo(MessageIn &message)
{
    mRegistrationAllowed = message.readInt8();
    mMinimumNameLength = message.readInt8();
    mMaximumNameLength = message.readInt8();
    mCaptchaUrl = message.readString();
    mCaptchaInstructions = message.readString();

    emit registrationInfoChanged();
}

void AccountClient::handleRegisterResponse(MessageIn &message)
{
    const int error = message.readInt8();

    if (error == ERRMSG_OK) {
        readServerInfo(message);
        if (mUsername != mPendingUsername) {
            mUsername = mPendingUsername;
            emit usernameChanged();
        }
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
        readServerInfo(message);
        if (mUsername != mPendingUsername) {
            mUsername = mPendingUsername;
            emit usernameChanged();
        }
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
        Character *ch = mCharacters.takeAt(mDeleteIndex);
        mCharacterListModel->setCharacters(mCharacters);
        delete ch;
        emit deleteCharacterSucceeded();
    } else {
        // Delete error messages are same as choose character error messages
        emit deleteCharacterFailed(error, chooseCharacterErrorMessage(error));
    }

    mDeleteIndex = -1;
}

void AccountClient::handleCharacterInfo(MessageIn &message)
{
    Character *character = new Character;

    const int slot = message.readInt8();
    const QString name = message.readString();
    const Being::BeingGender gender =
            static_cast<Being::BeingGender>(message.readInt8());
    const int hairStyle = message.readInt8();
    const int hairColor = message.readInt8();
    message.readInt16(); // character points
    message.readInt16(); // correction points

    character->setCharacterSlot(slot);
    character->setName(name);
    character->setGender(gender);
    character->setHairStyle(hairStyle, hairColor);

    while (message.unreadData()) {
        const unsigned id = message.readInt32();

        int base = message.readInt32() / 256.0;
        int mod = message.readInt32() / 256.0;

        Q_UNUSED(id)
        Q_UNUSED(base)
        Q_UNUSED(mod)

        // TODO: Extract the money attribute from here and display it
    }

    mCharacters.append(character);
    mCharacterListModel->setCharacters(mCharacters);

    emit characterInfoReceived(character);
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
        emit gameServerChanged();
        emit chatServerChanged();

        if (mPlayerName != mPendingPlayerName) {
            mPlayerName = mPendingPlayerName;
            emit playerNameChanged();
        }
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

void AccountClient::handleSaltResponse(MessageIn &message)
{
    // login with the recieved hash
    login(mPendingUsername, mPendingPassword, message.readByteArray());
    mPendingPassword.clear();
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
    case CREATE_INVALID_SLOT:
        return tr("Invalid slot");
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
