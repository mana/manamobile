/*
 *  Mana Mobile
 *  Copyright (C) 2010  Thorbjørn Lindeijer
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

#include "root.h"

#include "resourcemanager.h"

#include "mana/accountclient.h"
#include "mana/chatclient.h"
#include "mana/gameclient.h"

#include <QTimerEvent>

Root *Root::mInstance;

Root::Root(QObject *parent)
    : QObject(parent)
    , mAccountClient(new Mana::AccountClient(this))
    , mChatClient(new Mana::ChatClient(this))
    , mGameClient(new Mana::GameClient(this))
    , mResourceManager(new ResourceManager(this))
{
    Q_ASSERT(!mInstance);
    mInstance = this;

    mNetworkTrafficTimer = startTimer(100);
}

Root::~Root()
{
    delete mAccountClient;
    delete mChatClient;
    delete mGameClient;
    mInstance = 0;
}

void Root::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mNetworkTrafficTimer) {
        mAccountClient->service();
        mChatClient->service();
        mGameClient->service();
    }
}
