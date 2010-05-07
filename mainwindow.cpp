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

#include "loginwidget.h"
#include "serversettingsdialog.h"

#include <QCoreApplication>
#include <QMenuBar>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QStackedWidget *stack = new QStackedWidget(this);
    mLoginWidget = new LoginWidget(stack);

    stack->addWidget(mLoginWidget);
    setCentralWidget(stack);

    QMenu *menu = new QMenu(tr("Menu"));
    menu->addAction(tr("About Qt"), qApp, SLOT(aboutQt()));
    menu->addAction(tr("Full Screen"), this, SLOT(toggleFullScreen()));
    menu->addAction(tr("Settings"), this, SLOT(openSettings()));
    menuBar()->addMenu(menu);
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
