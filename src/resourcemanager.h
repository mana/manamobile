/*
 *  Mana Mobile
 *  Copyright (C) 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *  Copyright (C) 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
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

#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "mana/resource/pixmapresource.h"
#include "mana/resource/spritedef.h"

/**
 * This is meant to be a convenient abstraction on top of QNetworkAccessManager
 * for retrieving and managing resources.
 */
class ResourceManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dataUrl READ dataUrl WRITE setDataUrl NOTIFY dataUrlChanged)
    Q_PROPERTY(bool pathsLoaded READ pathsLoaded NOTIFY pathsLoadedChanged)

public:
    enum CustomAttribute {
        RequestedFile = QNetworkRequest::User
    };

    explicit ResourceManager(QObject *parent = 0);

    static ResourceManager *instance();

    QString dataUrl() const;
    void setDataUrl(const QString &url);

    bool pathsLoaded() const;


    QString path(const QString &key, const QString &value = QString()) const;
    QString spritePath() const;

    QNetworkReply *requestFile(const QString &fileName);

    void removeResource(Mana::Resource *resource);

    Q_INVOKABLE void cleanUpResources();

    Mana::SpriteDefinition *requestSpriteDefinition(const QString &path,
                                                    int variant = 0);

    Mana::PixmapResource *requestPixmap(const QString &path);

    static QNetworkRequest::Attribute requestedFileAttribute();

signals:
    void dataUrlChanged();
    void pathsLoadedChanged();

private slots:
    void pathsFileFinished();

private:
    QString mDataUrl;
    QNetworkAccessManager mNetworkAccessManager;
    QMap<QString, Mana::Resource *> mResources;

    bool mPathsLoaded;
    QMap<QString, QString> mPaths;

    static ResourceManager *mInstance;
};


inline ResourceManager *ResourceManager::instance()
{ return mInstance; }

inline QString ResourceManager::dataUrl() const
{ return mDataUrl; }

inline bool ResourceManager::pathsLoaded() const
{ return mPathsLoaded; }

inline QString ResourceManager::path(const QString &key,
                                     const QString &value) const
{ return mPaths.value(key, value); }

inline QString ResourceManager::spritePath() const
{ return path("sprites", "graphics/sprites/"); }

inline QNetworkRequest::Attribute ResourceManager::requestedFileAttribute()
{ return static_cast<QNetworkRequest::Attribute>(RequestedFile); }

#endif // RESOURCEMANAGER_H
