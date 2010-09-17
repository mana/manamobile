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

#ifndef CHOOSECHARACTERWIDGET_H
#define CHOOSECHARACTERWIDGET_H

#include <QWidget>

class QModelIndex;

namespace Ui {
class ChooseCharacterWidget;
}

class CharacterModel;
class LoginManager;

class ChooseCharacterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseCharacterWidget(LoginManager *loginManager,
                                   QWidget *parent = 0);
    ~ChooseCharacterWidget();

signals:
    void characterChosen();

private slots:
    void updateCharacters();
    void chooseCharacter(const QModelIndex &index);

    void onChooseCharacterFailed();

private:
    Ui::ChooseCharacterWidget *ui;
    LoginManager *mLoginManager;
    CharacterModel *mCharacterModel;
};

#endif // CHOOSECHARACTERWIDGET_H
