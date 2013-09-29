import QtQuick 2.0
import Mana 1.0

/**
 * This is the base component of a QML based Mana client. It sets up the
 * account, chat and game clients and provides access to them.
 */
Item {
    id: client

    property string serverName: "ManaSource!";

    property string host
    property int port

    property bool loggingIn: false
    property bool loggedIn: false
    property bool reconnecting: false

    signal reconnectFailed

    function connect(_host, _port) {
        reconnecting = false;
        host = _host;
        port = _port;
        accountClient.connect(_host, _port);
    }

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
        onConnected: {
            if (reconnecting)
                reconnect(gameClient.token);
            else
                requestRegistrationInfo();
        }

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

        onReconnectFailed: client.reconnectFailed();

        onChooseCharacterSucceeded: {
            // Connect to chat and game servers
            chatClient.connect(chatServerHost, chatServerPort);
            gameClient.playerName = accountClient.playerName;
            gameClient.connect(gameServerHost, gameServerPort);
        }

        onLoggedOut: loggedIn = false;
    }
    property ChatClient chatClient: ChatClient {
        onConnected: authenticate(accountClient.token);
    }
    property GameClient gameClient: GameClient {
        onConnected: authenticate(accountClient.token);
        onTokenReceived: {
            reconnecting = true;
            if (accountClient.connected)
                accountClient.reconnect(gameClient.token);
            else
                accountClient.connect(host, port);
        }
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
