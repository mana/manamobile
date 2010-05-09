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

#include "choosecharacterwidget.h"
#include "loginmanager.h"
#include "loginwidget.h"
#include "serversettingsdialog.h"

#include <QCoreApplication>
#include <QMenuBar>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mLoginManager(new LoginManager(this))
{
    mStack = new QStackedWidget(this);
    mLoginWidget = new LoginWidget(mLoginManager, mStack);
    mChooseCharacterWidget = new ChooseCharacterWidget(mLoginManager, mStack);

    mStack->addWidget(mLoginWidget);
    mStack->addWidget(mChooseCharacterWidget);

    setCentralWidget(mStack);

    QMenu *menu = new QMenu(tr("Menu"));
    menu->addAction(tr("About Qt"), qApp, SLOT(aboutQt()));
    menu->addAction(tr("Full Screen"), this, SLOT(toggleFullScreen()));
    menu->addAction(tr("Settings"), this, SLOT(openSettings()));
    menuBar()->addMenu(menu);

    connect(mLoginWidget, SIGNAL(loginSucceeded()),
            this, SLOT(onLoginSucceeded()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::toggleFullScreen()
{
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::openSettings()
{
    ServerSettingsDialog dialog(this);
    dialog.setServer(mLoginWidget->server());

    if (dialog.exec() == QDialog::Accepted)
        mLoginWidget->setServer(dialog.server());
}

void MainWindow::onLoginSucceeded()
{
    mStack->setCurrentWidget(mChooseCharacterWidget);
}
