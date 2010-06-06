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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ChooseCharacterWidget;
class GameWidget;
class LoginManager;
class LoginWidget;

class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void toggleFullScreen();
    void openSettings();

    void onLoginSucceeded();
    void onCharacterChosen();

private:
    QStackedWidget *mStack;

    LoginManager *mLoginManager;

    LoginWidget *mLoginWidget;
    ChooseCharacterWidget *mChooseCharacterWidget;
    GameWidget *mGameWidget;
};

#endif // MAINWINDOW_H
