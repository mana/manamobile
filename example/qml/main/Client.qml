import QtQuick 2.0
import Mana 1.0

/**
 * This is the base component of a QML based Mana client. It sets up the
 * account, chat and game clients and provides access to them.
 */
Item {
    property string serverName: "ManaSource!";

    property bool loggingIn: false
    property bool loggedIn: false

    function setDataUrl(dataUrl) {
        resourceManager.dataUrl = dataUrl;

        attributeDB.load();
        abilityDB.load();
        hairDB.load();
        itemDB.load();
        monsterDB.load();
        npcDB.load();
        raceDB.load();
    }

    property AccountClient accountClient: AccountClient {
        onConnected: requestRegistrationInfo();
        onLoginSucceeded: {
            setDataUrl(dataUrl);
            loggedIn = true
            loggingIn = false
        }
        onLoginFailed: loggingIn = false;
        onRegistrationSucceeded: {
            setDataUrl(dataUrl);
            loggedIn = true
            loggingIn = false
        }
        onRegistrationFailed: loggingIn = false;
        onChooseCharacterSucceeded: {
            // Connect to chat and game servers
            chatClient.connect(chatServerHost, chatServerPort);
            gameClient.connect(gameServerHost, gameServerPort);
        }
        onLoggedOut: loggedIn = false;
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
