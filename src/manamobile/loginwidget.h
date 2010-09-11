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

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <mana/manaclient.h>

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginManager;

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(LoginManager *loginManager, QWidget *parent = 0);
    ~LoginWidget();

    void setServer(const Mana::ServerAddress &server);
    const Mana::ServerAddress &server() const { return mServer; }

signals:
    void loginSucceeded();

private slots:
    void login();
    void loginFailed();
    void onLoginSucceeded();

    void connected();
    void disconnected();

private:
    Ui::LoginWidget *mUi;
    LoginManager *mLoginManager;
    Mana::ServerAddress mServer;
};

#endif // LOGINWIDGET_H
