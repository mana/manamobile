import QtQuick 2.0
import Mana 1.0

/**
 * This is the base component of a QML based Mana client. It sets up the
 * account, chat and game clients and provides access to them.
 */
Item {
    property string serverName: "ManaSource!";

    property AccountClient accountClient: AccountClient {
        onConnected: requestRegistrationInfo();
        onLoginSucceeded: {
            resourceManager.dataUrl = dataUrl;
            attributeDB.load();
            abilityDB.load();
            hairDB.load();
            itemDB.load();
            monsterDB.load();
            npcDB.load();
            raceDB.load();
        }
        onRegistrationSucceeded: resourceManager.dataUrl = dataUrl;
        onChooseCharacterSucceeded: {
            // Connect to chat and game servers
            chatClient.connect(chatServerHost, chatServerPort);
            gameClient.connect(gameServerHost, gameServerPort);
        }
    }
    property ChatClient chatClient: ChatClient {
        onConnected: authenticate(accountClient.token);
    }
    property GameClient gameClient: GameClient {
        playerName: accountClient.playerName;
        onConnected: authenticate(accountClient.token);
    }

    Timer {
        id: networkTimer
        interval: 100
        running: true
        repeat: true

        onTriggered: {
            accountClient.service();
            chatClient.service();
            gameClient.service();
        }
    }

    Timer {
        id: cleanUpTimer
        interval: 10000
        running: true
        repeat: true
        onTriggered: {
            resourceManager.cleanUpResources();
        }
    }
}
