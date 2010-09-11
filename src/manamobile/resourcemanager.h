/*
 *  Mana Mobile
 *  Copyright (C) 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QNetworkAccessManager>

/**
 * This class represents a resource that has been requested but may still need
 * to be downloaded.
 */
class PendingResource : public QObject
{
    Q_OBJECT

public:
    PendingResource(QNetworkReply *reply);

signals:
    void progress(int percentage);
    void finished();
    void error();

private slots:
    void onProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onFinished();

private:
    QNetworkReply *mReply;
};

/**
 * This is meant to be a convenient abstraction on top of QNetworkAccessManager
 * for retrieving and managing resources.
 */
class ResourceManager : public QObject
{
    Q_OBJECT

public:
    explicit ResourceManager(QObject *parent = 0);

    static ResourceManager *instance() { return mInstance; }

    void setDataUrl(const QString &url)
    { mDataUrl = url; }

    PendingResource *requestFile(const QString &fileName);

private:
    QString mDataUrl;
    QNetworkAccessManager mNetworkAccessManager;

    static ResourceManager *mInstance;
};

#endif // RESOURCEMANAGER_H
