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

#include "gamehandler.h"

#include "resourcemanager.h"

#include <QDebug>

GameHandler::GameHandler(Mana::GameClient *gameGlient, QObject *parent)
    : QObject(parent)
    , mGameClient(gameGlient)
{
    connect(mGameClient, SIGNAL(mapChanged(QString,int,int)),
            this, SLOT(changeMap(QString,int,int)));
}

GameHandler::~GameHandler()
{
}

void GameHandler::changeMap(const QString &name, int x, int y)
{
    qDebug() << "Arrived at" << name << x << y;

    QString fileName = QLatin1String("maps/");
    fileName += name;

    const QLatin1String mapExtension(".tmx");
    if (!fileName.endsWith(mapExtension))
        fileName += mapExtension;

    ResourceManager::instance()->requestFile(fileName);
}
