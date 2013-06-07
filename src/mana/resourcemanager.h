/*
 *  Mana Mobile
 *  Copyright (C) 2010-2013, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

namespace Mana {

class ImageResource;
class Resource;
class ResourceListModel;
class SpriteDefinition;

/**
 * This is meant to be a convenient abstraction on top of QNetworkAccessManager
 * for retrieving and managing resources.
 */
class ResourceManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dataUrl READ dataUrl WRITE setDataUrl NOTIFY dataUrlChanged)
    Q_PROPERTY(bool pathsLoaded READ pathsLoaded NOTIFY pathsLoadedChanged)

    Q_PROPERTY(Mana::ResourceListModel *resourceListModel READ resourceListModel CONSTANT)

public:
    enum CustomAttribute {
        RequestedFile = QNetworkRequest::User
    };

    explicit ResourceManager(QObject *parent = 0);
    ~ResourceManager();

    static ResourceManager *instance();

    QString dataUrl() const;
    void setDataUrl(const QString &url);

    bool pathsLoaded() const;

    ResourceListModel *resourceListModel() const;

    QString path(const QString &key, const QString &value = QString()) const;
    QString spritePath() const;

    QUrl resolve(const QString &path) const;

    QNetworkReply *requestFile(const QString &fileName);
    QNetworkReply *requestFile(const QUrl &url);

    void removeResource(Resource *resource);

    Q_INVOKABLE void cleanUpResources();

    SpriteDefinition *requestSpriteDefinition(const QString &path,
                                              int variant = 0);

    ImageResource *requestImage(const QString &path);

    static QNetworkRequest::Attribute requestedFileAttribute();

signals:
    void dataUrlChanged();
    void pathsLoadedChanged();

private slots:
    void pathsFileFinished();

private:
    template <class R> R *find(const QUrl &url);

    QString mDataUrl;
    QNetworkAccessManager mNetworkAccessManager;
    QHash<QUrl, Resource *> mResources;

    bool mPathsLoaded;
    QHash<QString, QString> mPaths;

    ResourceListModel *mResourceListModel;

    static ResourceManager *mInstance;
};


inline ResourceManager *ResourceManager::instance()
{ return mInstance; }

inline QString ResourceManager::dataUrl() const
{ return mDataUrl; }

inline bool ResourceManager::pathsLoaded() const
{ return mPathsLoaded; }

inline ResourceListModel *ResourceManager::resourceListModel() const
{ return mResourceListModel; }

inline QString ResourceManager::path(const QString &key,
                                     const QString &value) const
{ return mPaths.value(key, value); }

inline QString ResourceManager::spritePath() const
{ return path("sprites", "graphics/sprites/"); }

/**
 * Returns the URL for the resource located at \a path, relative from the
 * data URL.
 */
inline QUrl ResourceManager::resolve(const QString &path) const
{ return QUrl(mDataUrl).resolved(QUrl(path)); }

inline QNetworkRequest::Attribute ResourceManager::requestedFileAttribute()
{ return static_cast<QNetworkRequest::Attribute>(RequestedFile); }

template <class R>
inline R *ResourceManager::find(const QUrl &url)
{ return static_cast<R*>(mResources.value(url)); }

} // namespace Mana

#endif // RESOURCEMANAGER_H
