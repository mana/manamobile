/**
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

    void login(const QString &username, const QString &password);

signals:
    void connected();
    void disconnected();

private slots:
    void onReadyRead();

private:
    QByteArray mInputBuffer;
    Client *mClient;
};

#endif // LOGINMANAGER_H
