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

#include "choosecharacterwidget.h"
#include "ui_choosecharacterwidget.h"

#include "loginmanager.h"

#include <QAbstractListModel>
#include <QMessageBox>


class CharacterModel : public QAbstractListModel
{
public:
    explicit CharacterModel(QObject *parent = 0)
        : QAbstractListModel(parent)
    {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setCharacters(const QList<Mana::CharacterInfo> &characters)
    {
        mCharacters = characters;
        reset();
    }

    Mana::CharacterInfo characterAt(int index)
    {
        return mCharacters.at(index);
    }

private:
    QList<Mana::CharacterInfo> mCharacters;
};

int CharacterModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mCharacters.size();
}

QVariant CharacterModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == 0 && role == Qt::DisplayRole)
        return QString::fromStdString(mCharacters.at(index.row()).name);
    return QVariant();
}


ChooseCharacterWidget::ChooseCharacterWidget(LoginManager *loginManager,
                                             QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChooseCharacterWidget)
    , mLoginManager(loginManager)
    , mCharacterModel(new CharacterModel(this))
{
    ui->setupUi(this);
    ui->listView->setModel(mCharacterModel);

    connect(mLoginManager, SIGNAL(charactersChanged()),
            this, SLOT(updateCharacters()));
    connect(mLoginManager, SIGNAL(chooseCharacterFailed()),
            SLOT(onChooseCharacterFailed()));
    connect(mLoginManager, SIGNAL(chooseCharacterSucceeded()),
            SIGNAL(characterChosen()));
    connect(ui->listView, SIGNAL(activated(QModelIndex)),
            SLOT(chooseCharacter(QModelIndex)));
}

ChooseCharacterWidget::~ChooseCharacterWidget()
{
    delete ui;
}

void ChooseCharacterWidget::updateCharacters()
{
    mCharacterModel->setCharacters(mLoginManager->characters());
}

void ChooseCharacterWidget::chooseCharacter(const QModelIndex &index)
{
    Mana::CharacterInfo character = mCharacterModel->characterAt(index.row());
    mLoginManager->manaClient()->chooseCharacter(character);
}

void ChooseCharacterWidget::onChooseCharacterFailed()
{
    QMessageBox::critical(0, QCoreApplication::applicationName(),
                          mLoginManager->errorMessage());
}
