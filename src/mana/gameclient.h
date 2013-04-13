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
#include <QVector2D>

namespace Mana {

class AbilityListModel;
class AttributeListModel;
class Being;
class BeingListModel;
class Character;
class LogicDriver;
class NpcDialogManager;

/**
 * The game client allows interacting with the game server.
 *
 * The player name should be set so that the game server can identify who the
 * current player is.
 */
class GameClient : public ENetClient
{
    Q_OBJECT

    Q_PROPERTY(bool authenticated READ authenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(QString currentMap READ currentMap NOTIFY mapChanged)
    Q_PROPERTY(int playerStartX READ playerStartX NOTIFY mapChanged)
    Q_PROPERTY(int playerStartY READ playerStartY NOTIFY mapChanged)
    Q_PROPERTY(Mana::BeingListModel *beingListModel READ beingListModel CONSTANT)
    Q_PROPERTY(Mana::Character *player READ player NOTIFY playerChanged)
    Q_PROPERTY(QPointF playerWalkDirection READ playerWalkDirection WRITE setPlayerWalkDirection NOTIFY playerWalkDirectionChanged)

    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)

    Q_PROPERTY(Mana::AbilityListModel *abilityListModel READ abilityListModel CONSTANT)
    Q_PROPERTY(Mana::AttributeListModel *attributeListModel READ attributeListModel CONSTANT)
    Q_PROPERTY(Mana::NpcDialogManager *npcDialogManager READ npcDialogManager CONSTANT)

public:
    GameClient(QObject *parent = 0);
    ~GameClient();

    bool authenticated() const { return mAuthenticated; }

    QString currentMap() const { return mCurrentMap; }
    int playerStartX() const { return mPlayerStartX; }
    int playerStartY() const { return mPlayerStartY; }

    BeingListModel *beingListModel() const;
    Character *player() const;

    QPointF playerWalkDirection() const;
    void setPlayerWalkDirection(QPointF direction);

    QString playerName() const;
    void setPlayerName(const QString &name);

    Q_INVOKABLE void authenticate(const QString &token);
    Q_INVOKABLE void walkTo(int x, int y);
    Q_INVOKABLE void say(const QString &text);
    Q_INVOKABLE void useAbility(unsigned id, QPointF target);

    AbilityListModel *abilityListModel() const { return mAbilityListModel; }
    AttributeListModel *attributeListModel() const;
    NpcDialogManager *npcDialogManager() const { return mNpcDialogManager; }

signals:
    void authenticationFailed(const QString &errorMessage);

    void authenticatedChanged();
    void mapChanged(const QString &name, int x, int y);
    void playerChanged();
    void playerWalkDirectionChanged();

    void playerNameChanged();

    void kicked();

protected:
    void messageReceived(MessageIn &message);

private slots:
    void startedTalkingToNpc(int npcId);
    void nextNpcTalk(int npcId);
    void playerPositionChanged();
    void doNpcChoice(int npcId, int choice);

    void restoreWalkingSpeed();

private:
    void handleAuthenticationResponse(MessageIn &message);
    void handlePlayerMapChanged(MessageIn &message);

    void handleAbilityStatus(MessageIn &messageIn);
    void handleAbilityRemoved(MessageIn &messageIn);

    void handlePlayerAttributeChange(MessageIn &message);

    bool mAuthenticated;
    QString mCurrentMap;
    int mPlayerStartX;
    int mPlayerStartY;

    AbilityListModel *mAbilityListModel;
    AttributeListModel *mAttributeListModel;
    BeingListModel *mBeingListModel;
    NpcDialogManager *mNpcDialogManager;
    LogicDriver *mLogicDriver;
};

inline AttributeListModel *GameClient::attributeListModel() const
{
    return mAttributeListModel;
}

} // namespace Mana

#endif // GAMECLIENT_H
