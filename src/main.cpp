/*
 *  Mana Mobile
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *  Copyright (C) 2012  Erik Schilling
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

#include <enet/enet.h>

#include <QApplication>
#include <QMessageBox>
#include <QDeclarativeContext>
#include <qdeclarative.h>

#ifndef QT_NO_OPENGL
#include <QGLWidget>
#endif

#include "qmlapplicationviewer.h"

#include "mana/accountclient.h"
#include "mana/attributelistmodel.h"
#include "mana/being.h"
#include "mana/beinglistmodel.h"
#include "mana/characterlistmodel.h"
#include "mana/chatclient.h"
#include "mana/gameclient.h"
#include "mana/npcdialogmanager.h"
#include "mana/spritelistmodel.h"

#include "mana/resource/attributedb.h"
#include "mana/resource/hairdb.h"
#include "mana/resource/itemdb.h"
#include "mana/resource/monsterdb.h"
#include "mana/resource/npcdb.h"
#include "mana/resource/racedb.h"

#include "mapitem.h"
#include "resourcemanager.h"
#include "spriteitem.h"

#include "mana/settings.h"

static void registerTypes()
{
    qmlRegisterUncreatableType<Mana::ENetClient>("Mana", 1, 0, "ENetClient",
                                                 "Use a derived class");

    qmlRegisterType<Mana::AccountClient>("Mana", 1, 0, "AccountClient");
    qmlRegisterType<Mana::ChatClient>("Mana", 1, 0, "ChatClient");
    qmlRegisterType<Mana::GameClient>("Mana", 1, 0, "GameClient");
    qmlRegisterType<Mana::Settings>("Mana", 1, 0, "Settings");
    qmlRegisterType<SpriteItem>("Mana", 1, 0, "Sprite");

    qmlRegisterType<Mana::CharacterListModel>();
    qmlRegisterType<Mana::BeingListModel>();
    qmlRegisterUncreatableType<Mana::Being>("Mana", 1, 0, "Being",
                                            "Managed on C++ side");
    qmlRegisterUncreatableType<Mana::Character>("Mana", 1, 0, "Character",
                                                "Managed on C++ side");

    qmlRegisterType<Mana::NpcDialogManager>("Mana", 1, 0, "NpcDialogManager");

    qmlRegisterType<Mana::SpriteListModel>();
    qmlRegisterType<const Mana::SpriteReference>();

    qmlRegisterType<Mana::AttributeListModel>();
    qmlRegisterType<Mana::AttributeInfo>();
    qmlRegisterType<Mana::AttributeValue>();

    qmlRegisterType<ResourceManager>();
    qmlRegisterType<Mana::AttributeDB>();
    qmlRegisterType<Mana::ItemDB>();
    qmlRegisterType<Mana::ItemInfo>();
    qmlRegisterType<Mana::MonsterDB>();
    qmlRegisterType<Mana::NpcDB>();
    qmlRegisterType<Mana::RaceDB>();

    qmlRegisterType<MapItem>("Tiled", 1, 0, "TileMap");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Mana Mobile");
    app.setOrganizationDomain("manasource.org");
    app.setOrganizationName(QLatin1String("mana"));
    app.setApplicationVersion("0.1");

    if (enet_initialize() != 0)
    {
        QMessageBox::critical(0, app.applicationName(),
                              "An error occurred while initializing ENet.\n");
        return 1;
    }
    atexit(enet_deinitialize);

    registerTypes();

    QmlApplicationViewer viewer;
#ifndef QT_NO_OPENGL
    viewer.setViewport(new QGLWidget);
#endif
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setWindowTitle(app.applicationName());

    ResourceManager *resourceManager = new ResourceManager(&viewer);
    Mana::AttributeDB *attributeDB = new Mana::AttributeDB(&viewer);
    Mana::HairDB *hairDB = new Mana::HairDB(&viewer);
    Mana::ItemDB *itemDB = new Mana::ItemDB(&viewer);
    Mana::MonsterDB *monsterDB = new Mana::MonsterDB(&viewer);
    Mana::NpcDB *npcDB = new Mana::NpcDB(&viewer);
    Mana::RaceDB *raceDB = new Mana::RaceDB(&viewer);

    QStringList arguments = app.arguments();
    QString customServerListPath = "";
    for (int i = 1, max = arguments.length(); i < max; ++i) {
        if (arguments[i] == "--serverlist") {
            if (i + 1 < max)
                customServerListPath = arguments[++i];
            else
                qWarning() << "Missing argument for --serverlist";
        }
    }

    QDeclarativeContext *context = viewer.rootContext();
    context->setContextProperty("customServerListPath", customServerListPath);
    context->setContextProperty("resourceManager", resourceManager);
    context->setContextProperty("attributeDB", attributeDB);
    context->setContextProperty("hairDB", hairDB);
    context->setContextProperty("itemDB", itemDB);
    context->setContextProperty("monsterDB", monsterDB);
    context->setContextProperty("npcDB", npcDB);
    context->setContextProperty("raceDB", raceDB);

    viewer.setMainQmlFile(QLatin1String("qml/main/mobile.qml"));

#if defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6) || defined(Q_OS_SYMBIAN)
    viewer.showFullScreen();
#else
    viewer.show();
#endif

    return app.exec();
}
