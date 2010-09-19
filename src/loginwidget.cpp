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

#include "loginwidget.h"
#include "ui_loginwidget.h"

#include "loginmanager.h"

#include <QSettings>

const char * const USERNAME_KEY = "Login/Username";

LoginWidget::LoginWidget(LoginManager *loginManager, QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::LoginWidget),
    mLoginManager(loginManager)
{
    mServer.host = "testing.manasource.org";
    mServer.port = 9601;

    mUi->setupUi(this);

    QSettings settings;
    mUi->usernameEdit->setText(settings.value(USERNAME_KEY).toString());
    if (!mUi->usernameEdit->text().isEmpty())
        mUi->passwordEdit->setFocus();

    connect(mUi->buttonLogin, SIGNAL(clicked()), SLOT(login()));

    connect(mLoginManager, SIGNAL(connected()), SLOT(connected()));
    connect(mLoginManager, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(mLoginManager, SIGNAL(loginFailed()), SLOT(loginFailed()));
    connect(mLoginManager, SIGNAL(loginSucceeded()), SLOT(onLoginSucceeded()));
}

LoginWidget::~LoginWidget()
{
    delete mUi;
}

void LoginWidget::setServer(const Mana::ServerAddress &server)
{
    // If the server changed while already connected, disconnect
    if (mLoginManager->isConnected()
        && (mServer != server))
        mLoginManager->disconnectFromLoginServer();

    mServer = server;
}

void LoginWidget::login()
{
    mUi->errorLabel->setText(QString());
    mUi->buttonLogin->setEnabled(false);

    if (!mLoginManager->isConnected())
        mLoginManager->connectToLoginServer(mServer);
    else
        connected();
}

void LoginWidget::loginFailed()
{
    mUi->errorLabel->setText(mLoginManager->error());
    mUi->buttonLogin->setEnabled(true);
}

void LoginWidget::onLoginSucceeded()
{
    // Clear any previously displayed error
    mUi->errorLabel->setText(QString());

    // Remember the username for next time
    QSettings settings;
    settings.setValue(USERNAME_KEY, mUi->usernameEdit->text());

    emit loginSucceeded();
}

void LoginWidget::connected()
{
    mLoginManager->login(mUi->usernameEdit->text().trimmed(),
                         mUi->passwordEdit->text().trimmed());
}

void LoginWidget::disconnected()
{
    mUi->buttonLogin->setEnabled(true);
}
