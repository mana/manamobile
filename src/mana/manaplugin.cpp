/*
 * Mana QML plugin
 * Copyright (C) 2010  Thorbjørn Lindeijer
 * Copyright (C) 2013  Erik Schilling <ablu.erikschilling@googlemail.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "manaplugin.h"

#include "accountclient.h"
#include "attributelistmodel.h"
#include "beinglistmodel.h"
#include "characterlistmodel.h"
#include "chatclient.h"
#include "enetclient.h"
#include "gameclient.h"
#include "mapitem.h"
#include "npcdialogmanager.h"
#include "settings.h"
#include "spriteitem.h"
#include "spritelistmodel.h"

#include "resource/abilitydb.h"
#include "resource/attributedb.h"
#include "resource/hairdb.h"
#include "resource/itemdb.h"
#include "resourcelistmodel.h"
#include "resourcemanager.h"
#include "resource/monsterdb.h"
#include "resource/npcdb.h"
#include "resource/racedb.h"

#include <QQmlEngine>
#include <QQmlContext>

ManaPlugin::ManaPlugin(QObject *parent) :
    QQmlExtensionPlugin(parent)
{
}

void ManaPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Mana::ResourceManager *resourceManager = new Mana::ResourceManager(engine);
    Mana::AbilityDB *abilityDB = new Mana::AbilityDB(engine);
    Mana::AttributeDB *attributeDB = new Mana::AttributeDB(engine);
    Mana::HairDB *hairDB = new Mana::HairDB(engine);
    Mana::ItemDB *itemDB = new Mana::ItemDB(engine);
    Mana::MonsterDB *monsterDB = new Mana::MonsterDB(engine);
    Mana::NpcDB *npcDB = new Mana::NpcDB(engine);
    Mana::RaceDB *raceDB = new Mana::RaceDB(engine);

    QQmlContext *context = engine->rootContext();
    context->setContextProperty("resourceManager", resourceManager);
    context->setContextProperty("abilityDB", abilityDB);
    context->setContextProperty("attributeDB", attributeDB);
    context->setContextProperty("hairDB", hairDB);
    context->setContextProperty("itemDB", itemDB);
    context->setContextProperty("monsterDB", monsterDB);
    context->setContextProperty("npcDB", npcDB);
    context->setContextProperty("raceDB", raceDB);

    int errorCode = enet_initialize();
    Q_ASSERT(errorCode == 0);
    atexit(enet_deinitialize);
}

void ManaPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Mana"));

    qmlRegisterUncreatableType<Mana::ENetClient>(uri, 1, 0, "ENetClient",
                                                 "Use a derived class");

    qmlRegisterType<Mana::AccountClient>(uri, 1, 0, "AccountClient");
    qmlRegisterType<Mana::ChatClient>(uri, 1, 0, "ChatClient");
    qmlRegisterType<Mana::GameClient>(uri, 1, 0, "GameClient");
    qmlRegisterType<Mana::Settings>(uri, 1, 0, "Settings");
    qmlRegisterType<Mana::SpriteItem>(uri, 1, 0, "Sprite");
    qmlRegisterUncreatableType<Mana::Action>(uri, 1, 0, "Action",
                                             "Only exposed for direction enum");

    qmlRegisterType<Mana::CharacterListModel>();
    qmlRegisterType<Mana::BeingListModel>();
    qmlRegisterUncreatableType<Mana::Being>(uri, 1, 0, "Being",
                                            "Managed on C++ side");
    qmlRegisterType<Mana::Character>(uri, 1, 0, "Character");

    qmlRegisterType<Mana::NpcDialogManager>(uri, 1, 0, "NpcDialogManager");

    qmlRegisterType<Mana::SpriteListModel>();
    qmlRegisterType<const Mana::SpriteReference>();

    qmlRegisterType<Mana::AttributeListModel>();
    qmlRegisterType<Mana::AttributeInfo>();
    qmlRegisterType<Mana::AttributeValue>();

    qmlRegisterType<Mana::ResourceManager>();
    qmlRegisterUncreatableType<Mana::Resource>(uri, 1, 0, "Resource",
                                               "Base of all resources");
    qmlRegisterType<Mana::ResourceListModel>();

    qmlRegisterType<Mana::AbilityDB>();
    qmlRegisterType<Mana::AttributeDB>();
    qmlRegisterType<Mana::ItemDB>();
    qmlRegisterType<Mana::ItemInfo>();
    qmlRegisterType<Mana::MonsterDB>();
    qmlRegisterType<Mana::NpcDB>();
    qmlRegisterType<Mana::RaceDB>();
    qmlRegisterType<Mana::HairInfo>();

    qmlRegisterType<Mana::MapItem>(uri, 1, 0, "TileMap");
}
