/*
 * Mana QML plugin
 * Copyright (C) 2010  Thorbjørn Lindeijer
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

#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "enetclient.h"

#include <QPoint>
#include <QStringList>
#include <QVector2D>

namespace Mana {

class AbilityListModel;
class AttributeListModel;
class Being;
class BeingListModel;
class Character;
class InventoryListModel;
class LogicDriver;
class MapResource;
class NPC;

/**
 * The game client allows interacting with the game server.
 *
 * The player name should be set so that the client can identify who the
 * current player is.
 */
class GameClient : public ENetClient
{
    Q_OBJECT

    Q_PROPERTY(bool authenticated READ authenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(QString currentMapName READ currentMapName NOTIFY mapChanged)
    Q_PROPERTY(Mana::MapResource *currentMapResource READ currentMapResource NOTIFY mapChanged)
    Q_PROPERTY(int playerStartX READ playerStartX NOTIFY mapChanged)
    Q_PROPERTY(int playerStartY READ playerStartY NOTIFY mapChanged)
    Q_PROPERTY(Mana::Character *player READ player NOTIFY playerChanged)
    Q_PROPERTY(QPointF playerWalkDirection READ playerWalkDirection WRITE setPlayerWalkDirection NOTIFY playerWalkDirectionChanged)

    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)

    Q_PROPERTY(QString npcMessage READ npcMessage NOTIFY npcMessageChanged)
    Q_PROPERTY(QStringList npcChoices READ npcChoices NOTIFY npcChoicesChanged)
    Q_PROPERTY(Mana::NPC *npc READ npc NOTIFY npcChanged)
    Q_PROPERTY(NpcState npcState READ npcState NOTIFY npcStateChanged)

    Q_PROPERTY(Mana::AbilityListModel *abilityListModel READ abilityListModel CONSTANT)
    Q_PROPERTY(Mana::AttributeListModel *attributeListModel READ attributeListModel CONSTANT)
    Q_PROPERTY(Mana::BeingListModel *beingListModel READ beingListModel CONSTANT)
    Q_PROPERTY(Mana::InventoryListModel *inventoryListModel READ inventoryListModel CONSTANT)

    Q_ENUMS(NpcState)

public:
    enum NpcState {
        NoNpc,
        NpcAwaitNext,
        NpcAwaitChoice
    };

    GameClient(QObject *parent = 0);
    ~GameClient();

    bool authenticated() const { return mAuthenticated; }

    QString currentMapName() const { return mCurrentMap; }
    MapResource *currentMapResource() const { return mMapResource; }
    int playerStartX() const { return mPlayerStartX; }
    int playerStartY() const { return mPlayerStartY; }

    Character *player() const;

    QPointF playerWalkDirection() const;
    void setPlayerWalkDirection(QPointF direction);

    QString playerName() const;
    void setPlayerName(const QString &name);

    QString npcMessage() const { return mNpcMessage; }
    QStringList npcChoices() const { return mNpcChoices; }
    NPC *npc() const { return mNpc; }
    NpcState npcState() const { return mNpcState; }

    Q_INVOKABLE void authenticate(const QString &token);
    Q_INVOKABLE void walkTo(int x, int y);
    Q_INVOKABLE void say(const QString &text);

    Q_INVOKABLE void talkToNpc(Mana::Being *being);
    Q_INVOKABLE void nextNpcMessage();
    Q_INVOKABLE void chooseNpcOption(int choice);

    Q_INVOKABLE void useAbilityOnPoint(unsigned id, int x, int y);
    Q_INVOKABLE void useAbilityOnDirection(unsigned id);

    AbilityListModel *abilityListModel() const;
    AttributeListModel *attributeListModel() const;
    BeingListModel *beingListModel() const;
    InventoryListModel *inventoryListModel() const;

    Q_INVOKABLE void equip(unsigned slot);
    Q_INVOKABLE void unequip(unsigned slot);

signals:
    void authenticationFailed(const QString &errorMessage);

    void authenticatedChanged();
    void mapChanged(const QString &name, int x, int y);
    void playerChanged();
    void playerWalkDirectionChanged();

    void playerNameChanged();

    void npcMessageChanged();
    void npcChoicesChanged();
    void npcChanged();
    void npcStateChanged();

    void kicked();

protected:
    void messageReceived(MessageIn &message);

private slots:
    void update(qreal deltaTime);

private:
    void updatePlayer(qreal deltaTime);
    void playerPositionChanged();
    void restoreWalkingSpeed();

    void handleAuthenticationResponse(MessageIn &message);
    void handlePlayerMapChanged(MessageIn &message);

    void handleBeingEnter(MessageIn &message);
    void handleBeingLeave(MessageIn &message);
    void handleBeingDirChange(MessageIn &message);
    void handleBeingsMove(MessageIn &message);
    void handleBeingLooksChange(MessageIn &message);
    void handleBeingActionChange(MessageIn &message);
    void handleBeingSay(MessageIn &message);
    void handleBeingAbilityOnPoint(MessageIn &message);
    void handleBeingAbilityOnBeing(MessageIn &message);
    void handleBeingAbilityOnDirection(MessageIn &message);

    void handleNpcMessage(MessageIn &message);
    void handleNpcClose(MessageIn &message);
    void handleNpcChoice(MessageIn &message);

    void handleAbilityStatus(MessageIn &messageIn);
    void handleAbilityRemoved(MessageIn &messageIn);

    void handlePlayerAttributeChange(MessageIn &message);

    void handleInventory(MessageIn &message);
    void handleInventoryFull(MessageIn &message);
    void handleEquip(MessageIn &message);
    void handleUnEquip(MessageIn &message);

    bool mAuthenticated;
    QString mCurrentMap;
    MapResource *mMapResource;
    int mPlayerStartX;
    int mPlayerStartY;

    QString mPlayerName;
    Character *mPlayerCharacter;
    QVector2D mPlayerWalkDirection;

    NpcState mNpcState;
    QString mNpcMessage;
    QStringList mNpcChoices;
    NPC *mNpc;

    AbilityListModel *mAbilityListModel;
    AttributeListModel *mAttributeListModel;
    BeingListModel *mBeingListModel;
    InventoryListModel *mInventoryListModel;
    LogicDriver *mLogicDriver;
};


inline AbilityListModel *GameClient::abilityListModel() const
{
    return mAbilityListModel;
}

inline AttributeListModel *GameClient::attributeListModel() const
{
    return mAttributeListModel;
}
inline BeingListModel *GameClient::beingListModel() const
{
    return mBeingListModel;
}

inline InventoryListModel *GameClient::inventoryListModel() const
{
    return mInventoryListModel;
}

} // namespace Mana

#endif // GAMECLIENT_H
