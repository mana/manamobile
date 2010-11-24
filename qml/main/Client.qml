import Qt 4.7
import Mana 1.0

/**
 * This is the base component of a QML based Mana client. It sets up the
 * account, chat and game clients and provides access to them.
 */
Item {
    property string serverHost: "testing.manasource.org";
    property int serverPort: 9601;

    property variant accountClient: AccountClient {
        onConnected: requestRegistrationInfo();
        onLoginSucceeded: resourceManager.dataUrl = dataUrl;
        onRegistrationSucceeded: resourceManager.dataUrl = dataUrl;
        onChooseCharacterSucceeded: {
            // Connect to chat and game servers
            chatClient.connect(chatServerHost, chatServerPort);
            gameClient.connect(gameServerHost, gameServerPort);
        }
    }
    property variant chatClient: ChatClient {
        onConnected: authenticate(accountClient.token);
    }
    property variant gameClient: GameClient {
        playerName: accountClient.playerName;
        onConnected: authenticate(accountClient.token);
    }

    // Connect to the default server on startup
    Component.onCompleted: accountClient.connect(serverHost, serverPort);

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
}
