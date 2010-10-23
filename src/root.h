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

#ifndef ROOT_H
#define ROOT_H

#include <QObject>

namespace Mana {
class AccountClient;
class ChatClient;
class GameClient;
}

class ResourceManager;

/**
 * The root of the client. Provides access to all the parts that run the game.
 *
 * It is the root context object of the QML scene, making all its properties
 * globally available.
 */
class Root : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Mana::AccountClient *accountClient READ accountClient CONSTANT)
    Q_PROPERTY(Mana::ChatClient *chatClient READ chatClient CONSTANT)
    Q_PROPERTY(Mana::GameClient *gameClient READ gameClient CONSTANT)
    Q_PROPERTY(ResourceManager *resourceManager READ resourceManager CONSTANT)

public:
    explicit Root(QObject *parent = 0);
    ~Root();

    static Root *instance() { return mInstance; }

    Mana::AccountClient *accountClient() const { return mAccountClient; }
    Mana::ChatClient *chatClient() const { return mChatClient; }
    Mana::GameClient *gameClient() const { return mGameClient; }

    ResourceManager *resourceManager() const { return mResourceManager; }

protected:
    void timerEvent(QTimerEvent *);

private:
    int mNetworkTrafficTimer;

    Mana::AccountClient *mAccountClient;
    Mana::ChatClient *mChatClient;
    Mana::GameClient *mGameClient;

    ResourceManager *mResourceManager;

    static Root *mInstance;
};

#endif // ROOT_H
