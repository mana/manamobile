/*
 * manalib
 * Copyright 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
 *
 * This file is part of manalib.
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
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "imageresource.h"

#include "mana/resource/resource.h"
#include "mana/resourcemanager.h"

#include <QDebug>
#include <QNetworkReply>
#include <QQuickWindow>
#include <QSGTexture>

using namespace Mana;

ImageResource::ImageResource(const QUrl &url, QObject *parent)
    : Resource(url, parent)
    , mImage(0)
    , mTexture(0)
{
    QString path = url.path(QUrl::FullyDecoded);

    int pos = path.indexOf(QLatin1Char('|'));

    QString palettes;
    if (pos != -1)
        palettes = path.mid(pos + 1);

    // TODO: Dye

    const QString pathWithoutDye = path.left(pos);
    QUrl urlWithoutDye = url;
    urlWithoutDye.setPath(pathWithoutDye, QUrl::DecodedMode);

    ResourceManager *resourceManager = ResourceManager::instance();
    QNetworkReply *reply = resourceManager->requestFile(urlWithoutDye);
    connect(reply, SIGNAL(finished()), this, SLOT(imageFinished()));
    setStatus(Loading);
}

ImageResource::~ImageResource()
{
    delete mImage;

    if (mTexture)
        mTexture->deleteLater();
}

/**
 * Returns the image as a scene graph texture.
 */
QSGTexture *ImageResource::texture(const QQuickWindow *window) const
{
    if (!mTexture && mImage)
        mTexture = window->createTextureFromImage(*mImage);

    return mTexture;
}

void ImageResource::imageFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Failed to download image:" << reply->url() << "\n"
                   << reply->errorString();
        setStatus(Error);
        return;
    }

    mImage = new QImage;
    bool success = mImage->loadFromData(reply->readAll());
    setStatus(success ? Ready : Error);
}
