/*
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LoginManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSettings();

    void login();
    void loginFailed();
    void loginSucceeded();

    void connected();
    void disconnected();

private:
    Ui::MainWindow *mUi;
    LoginManager *mLoginManager;
    QString mHost;
    quint16 mPort;
};

#endif // MAINWINDOW_H
