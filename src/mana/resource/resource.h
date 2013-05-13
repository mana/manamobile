/*
 * manalib
 * Copyright 2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QObject>
#include <QUrl>

namespace Mana {

class Resource : public QObject
{
    Q_OBJECT

    Q_ENUMS(Status)

    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    enum Status {
        Null,
        Ready,
        Error,
        Loading
    };

    enum OrphanPolicy {
        DeleteLater,
        DeleteImmediately
    };

    Resource(const QUrl &url, QObject *parent = 0);

    virtual ~Resource();

    void incRef();
    void decRef(OrphanPolicy orphanPolicy = DeleteLater);

    const QUrl &url() const;

    int refCount() const;

    unsigned releaseTime() const;

    Status status() const;
    void setStatus(Status newStatus);

signals:
    void statusChanged(Resource::Status newStatus);

private:
    unsigned mRefCount;
    QUrl mUrl;
    // Time when the resource expired
    unsigned mReleaseTime;
    Status mStatus;
};


inline void Resource::incRef()
{ ++mRefCount; }

inline const QUrl &Resource::url() const
{ return mUrl; }

inline int Resource::refCount() const
{ return mRefCount; }

inline unsigned Resource::releaseTime() const
{ return mReleaseTime; }

inline Resource::Status Resource::status() const
{ return mStatus; }

}

#endif // RESOURCE_H
