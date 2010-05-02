/*
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
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

void ServerSettingsDialog::setHost(const QString &host)
{
    ui->hostEdit->setText(host);
}

QString ServerSettingsDialog::host() const
{
    return ui->hostEdit->text();
}

void ServerSettingsDialog::setPort(quint16 port)
{
    ui->portSpinBox->setValue(port);
}

quint16 ServerSettingsDialog::port() const
{
    return (quint16) ui->portSpinBox->value();
}
