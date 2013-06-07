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

#include "resource.h"

#include "mana/resourcemanager.h"

#include <QDateTime>

using namespace Mana;

Resource::Resource(const QUrl &url, QObject *parent)
    : QObject(parent)
    , mRefCount(0)
    , mUrl(url)
    , mReleaseTime(0)
    , mStatus(Null)
{
}

Resource::~Resource()
{
}

void Resource::incRef()
{
    ++mRefCount;
    emit refCountChanged();
}

void Resource::setStatus(Status newStatus)
{
    if (newStatus != mStatus) {
        mStatus = newStatus;
        emit statusChanged(newStatus);
    }
}

void Resource::decRef(OrphanPolicy orphanPolicy)
{
    Q_ASSERT(mRefCount > 0);

    --mRefCount;
    emit refCountChanged();

    if (mRefCount > 0)
        return;

    if (orphanPolicy == DeleteLater)
        mReleaseTime = QDateTime::currentMSecsSinceEpoch();
    else
        ResourceManager::instance()->removeResource(this);
}
