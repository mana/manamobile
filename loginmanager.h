/*
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QObject>

class Client;

class LoginManager : public QObject
{
    Q_OBJECT

public:
    explicit LoginManager(QObject *parent = 0);

    void connectToLoginServer(const QString &host, quint16 port);
    void disconnectFromLoginServer();
    bool isConnected() const;

    void login(const QString &username, const QString &password);

    QString errorMessage() const { return mError; }

signals:
    void connected();
    void disconnected();

    void loginFailed();
    void loginSucceeded();

private slots:
    void handleMessage(const QByteArray &message);

private:
    QString mError;
    Client *mClient;
};

#endif // LOGINMANAGER_H
