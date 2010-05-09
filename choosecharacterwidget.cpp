/*
 * Tiled Map Editor (Qt)
 * Copyright 2009 Tiled (Qt) developers (see AUTHORS file)
 *
 * This file is part of Tiled (Qt).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include "choosecharacterwidget.h"
#include "ui_choosecharacterwidget.h"

#include "loginmanager.h"

#include <QAbstractListModel>


class CharacterModel : public QAbstractListModel
{
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setCharacters(const QList<CharacterInfo> &characters)
    {
        mCharacters = characters;
        reset();
    }

private:
    QList<CharacterInfo> mCharacters;
};

int CharacterModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mCharacters.size();
}

QVariant CharacterModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == 0 && role == Qt::DisplayRole)
        return mCharacters.at(index.row()).name;
    return QVariant();
}


ChooseCharacterWidget::ChooseCharacterWidget(LoginManager *loginManager,
                                             QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChooseCharacterWidget)
    , mLoginManager(loginManager)
    , mCharacterModel(new CharacterModel)
{
    ui->setupUi(this);
    ui->listView->setModel(mCharacterModel);

    connect(mLoginManager, SIGNAL(charactersChanged()),
            this, SLOT(updateCharacters()));
}

ChooseCharacterWidget::~ChooseCharacterWidget()
{
    delete ui;
}

void ChooseCharacterWidget::updateCharacters()
{
    mCharacterModel->setCharacters(mLoginManager->characters());
}
