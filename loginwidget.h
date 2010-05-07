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

#include "client.h"

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginManager;

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

    void setServer(const ServerAddress &server);
    const ServerAddress &server() const { return mServer; }

    LoginManager *loginManager() const { return mLoginManager; }

private slots:
    void login();
    void loginFailed();
    void loginSucceeded();

    void connected();
    void disconnected();

private:
    Ui::LoginWidget *mUi;
    LoginManager *mLoginManager;
    ServerAddress mServer;
};

#endif // LOGINWIDGET_H
