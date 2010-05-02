/**
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "loginmanager.h"
#include "serversettingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mLoginManager(new LoginManager(this)),
    mHost("server.themanaworld.org"),
    mPort(6901)
{
    mUi->setupUi(this);

    connect(mUi->actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(mUi->actionSettings, SIGNAL(triggered()), SLOT(openSettings()));
    connect(mUi->buttonLogin, SIGNAL(clicked()), SLOT(login()));

    connect(mLoginManager, SIGNAL(connected()), SLOT(connected()));
    connect(mLoginManager, SIGNAL(disconnected()), SLOT(disconnected()));
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::openSettings()
{
    ServerSettingsDialog dialog;
    dialog.exec();

    mHost = dialog.host();
    mPort = dialog.port();
}

void MainWindow::login()
{
    mLoginManager->connectToLoginServer(mHost, mPort);
    mUi->buttonLogin->setEnabled(false);
    mUi->buttonLogin->setText(tr("Connecting..."));
}

void MainWindow::connected()
{
    mUi->buttonLogin->setText(tr("Connected!"));

    mLoginManager->login(mUi->usernameEdit->text(),
                         mUi->passwordEdit->text());
}

void MainWindow::disconnected()
{
    mUi->buttonLogin->setText(tr("Login"));
    mUi->buttonLogin->setEnabled(true);
}
