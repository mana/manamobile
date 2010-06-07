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

#include "gamewidget.h"

#include "resourcemanager.h"

#include <mana/chathandlerinterface.h>
#include <mana/gamehandlerinterface.h>
#include <mana/manaclient.h>

#include <QDebug>

class GameHandler : public Mana::GameHandlerInterface
{
public:
    void connected() {}
    void disconnected() {}

    void mapChanged(const std::string &name, int x, int y)
    {
        qDebug() << "Arrived at" << name.c_str() << x << y;

        QString fileName = QLatin1String("maps/");
        fileName += QString::fromStdString(name);

        const QLatin1String mapExtension(".tmx");
        if (!fileName.endsWith(mapExtension))
            fileName += mapExtension;

        ResourceManager::instance()->requestFile(fileName);
    }
};

class ChatHandler : public Mana::ChatHandlerInterface
{
public:
    void connected() {}
    void disconnected() {}
};

GameWidget::GameWidget(Mana::ManaClient *manaClient, QWidget *parent)
    : QWidget(parent)
    , mGameHandler(new GameHandler)
    , mChatHandler(new ChatHandler)
{
    manaClient->setGameHandler(mGameHandler);
    manaClient->setChatHandler(mChatHandler);
}
