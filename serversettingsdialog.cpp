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

#include "serversettingsdialog.h"
#include "ui_serversettingsdialog.h"

ServerSettingsDialog::ServerSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSettingsDialog)
{
    ui->setupUi(this);
}

ServerSettingsDialog::~ServerSettingsDialog()
{
    delete ui;
}

void ServerSettingsDialog::setServer(const Mana::ServerAddress &server)
{
    ui->hostEdit->setText(QString::fromStdString(server.host));
    ui->portSpinBox->setValue(server.port);
}

Mana::ServerAddress ServerSettingsDialog::server() const
{
    Mana::ServerAddress server;
    server.host = ui->hostEdit->text().toStdString();
    server.port = ui->portSpinBox->value();
    return server;
}
