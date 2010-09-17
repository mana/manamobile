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

#include "resourcemanager.h"

#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

ResourceManager *ResourceManager::mInstance;

PendingResource::PendingResource(QNetworkReply *reply)
    : mReply(reply)
{
    connect(mReply, SIGNAL(finished()), SIGNAL(finished()));
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)),
            SIGNAL(error()));
    connect(mReply, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(onProgress(qint64,qint64)));
}

void PendingResource::onProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << (int) ((double) bytesReceived / bytesTotal * 100);
    emit progress((int) ((double) bytesReceived / bytesTotal * 100));
}

void PendingResource::onFinished()
{
    qDebug() << Q_FUNC_INFO << mReply->url() << mReply->errorString();
}

ResourceManager::ResourceManager(QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

PendingResource *ResourceManager::requestFile(const QString &fileName)
{
    qDebug() << Q_FUNC_INFO << fileName;

    const QNetworkRequest request(mDataUrl + fileName);
    QNetworkReply *reply = mNetworkAccessManager.get(request);
    PendingResource *pendingResource = new PendingResource(reply);
    return pendingResource;
}
