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

#include "accounthandler.h"

#include "characterlistmodel.h"
#include "resourcemanager.h"
#include "root.h"
#include "safeassert.h"

#include <mana/protocol.h>

#include <QDebug>

AccountHandler::AccountHandler(Mana::AccountClient *client, QObject *parent)
    : QObject(parent)
    , mAccountClient(client)
    , mCharacterListModel(new CharacterListModel(this))
{
    connect(mAccountClient, SIGNAL(connected()),
            this, SIGNAL(connected()));
    connect(mAccountClient, SIGNAL(connected()),
            this, SIGNAL(isConnectedChanged()));
    connect(mAccountClient, SIGNAL(disconnected()),
            this, SIGNAL(disconnected()));
    connect(mAccountClient, SIGNAL(disconnected()),
            this, SIGNAL(isConnectedChanged()));

    connect(mAccountClient, SIGNAL(loginSucceeded()),
            this, SLOT(onLoginSucceeded()));
    connect(mAccountClient, SIGNAL(loginFailed(int)),
            this, SLOT(onLoginFailed(int)));
    connect(mAccountClient, SIGNAL(characterInfoReceived(Mana::CharacterInfo)),
            this, SLOT(onCharacterInfoReceived(Mana::CharacterInfo)));
    connect(mAccountClient, SIGNAL(chooseCharacterSucceeded()),
            this, SIGNAL(chooseCharacterSucceeded()));
    connect(mAccountClient, SIGNAL(chooseCharacterFailed(int)),
            this, SLOT(onChooseCharacterFailed(int)));
}

AccountHandler::~AccountHandler()
{
}

bool AccountHandler::isConnected() const
{
    return mAccountClient->isConnected();
}

void AccountHandler::chooseCharacter(int index)
{
    SAFE_ASSERT(index > 0 || index < mCharacters.size(), return);

    qDebug() << Q_FUNC_INFO << index;
    mAccountClient->chooseCharacter(mCharacters.at(index));
}

void AccountHandler::onLoginSucceeded()
{
    // Communicate the data url to the resource manager
    Root::instance()->resourceManager()->setDataUrl(mAccountClient->dataUrl());

    emit loginSucceeded();
}

void AccountHandler::onLoginFailed(int error)
{
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

    qDebug() << Q_FUNC_INFO << error << mError;

    emit errorChanged();
    emit loginFailed();
}

void AccountHandler::onChooseCharacterFailed(int error)
{
    switch (error) {
    default:
        mError = tr("Unknown error");
        break;
    case Mana::ERRMSG_NO_LOGIN:
        mError = tr("You don't seem to be logged in, please try again");
        break;
    case Mana::ERRMSG_INVALID_ARGUMENT:
        mError = tr("No such character");
        break;
    case Mana::ERRMSG_FAILURE:
        mError = tr("No game server found for the map the character is on");
        break;
    }

    qDebug() << Q_FUNC_INFO << error << mError;

    emit errorChanged();
    emit chooseCharacterFailed();
}

void AccountHandler::onCharacterInfoReceived(const Mana::CharacterInfo &info)
{
    mCharacters.append(info);
    mCharacterListModel->setCharacters(mCharacters);
    emit charactersChanged();
}
