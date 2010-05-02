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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "loginmanager.h"
#include "serversettingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mLoginManager(new LoginManager(this)),
    mHost("testing.manasource.org"),
    mPort(9601)
{
    mUi->setupUi(this);

    connect(mUi->actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(mUi->actionSettings, SIGNAL(triggered()), SLOT(openSettings()));
    connect(mUi->actionFullScreen, SIGNAL(triggered()), SLOT(showFullScreen()));
    connect(mUi->buttonLogin, SIGNAL(clicked()), SLOT(login()));

    connect(mLoginManager, SIGNAL(connected()), SLOT(connected()));
    connect(mLoginManager, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(mLoginManager, SIGNAL(loginFailed()), SLOT(loginFailed()));
    connect(mLoginManager, SIGNAL(loginSucceeded()), SLOT(loginSucceeded()));
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::openSettings()
{
    ServerSettingsDialog dialog(this);
    dialog.setHost(mHost);
    dialog.setPort(mPort);

    if (dialog.exec() == QDialog::Accepted) {
        // If the server changed while already connected, disconnect
        if (mLoginManager->isConnected()
            && (mHost != dialog.host() ||
                mPort != dialog.port()))
            mLoginManager->disconnectFromLoginServer();

        mHost = dialog.host();
        mPort = dialog.port();
    }
}

void MainWindow::fullScreen()
{
    setWindowState(Qt::WindowFullScreen);
}

void MainWindow::login()
{
    mUi->errorLabel->setText(QString());
    mUi->buttonLogin->setEnabled(false);

    if (!mLoginManager->isConnected())
        mLoginManager->connectToLoginServer(mHost, mPort);
    else
        connected();
}

void MainWindow::loginFailed()
{
    mUi->errorLabel->setText(mLoginManager->errorMessage());
    mUi->buttonLogin->setEnabled(true);
}

void MainWindow::loginSucceeded()
{
    mUi->errorLabel->setStyleSheet("color: green;");
    mUi->errorLabel->setText(tr("Login succesful!"));
}

void MainWindow::connected()
{
    mLoginManager->login(mUi->usernameEdit->text().trimmed(),
                         mUi->passwordEdit->text().trimmed());
}

void MainWindow::disconnected()
{
    mUi->buttonLogin->setEnabled(true);
}
