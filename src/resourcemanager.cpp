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

ResourceManager::ResourceManager(QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(!mInstance);
    mInstance = this;
}

void ResourceManager::setDataUrl(const QString &url)
{
    if (mDataUrl == url)
        return;

    mDataUrl = url;
    emit dataUrlChanged();
}

QNetworkReply *ResourceManager::requestFile(const QString &fileName)
{
    qDebug() << Q_FUNC_INFO << fileName;

    const QNetworkRequest request(mDataUrl + fileName);
    QNetworkReply *reply = mNetworkAccessManager.get(request);
    return reply;
}
