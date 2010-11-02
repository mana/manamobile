import Qt 4.7
import Mana 1.0 as Mana

/**
 * This is the base component of a QML based Mana client. It sets up the
 * account, chat and game clients and provides access to them.
 */
Item {
    property string serverHost: "testing.manasource.org";
    property int serverPort: 9601;

    // Make the clients globally available
    property alias accountClient: accountClient;
    property alias chatClient: chatClient;
    property alias gameClient: gameClient;

    // Connect to the default server on startup
    Component.onCompleted: accountClient.connect(serverHost, serverPort);

    Mana.AccountClient {
        id: accountClient

        onConnected: requestRegistrationInfo();
        onLoginSucceeded: resourceManager.dataUrl = dataUrl;
        onRegistrationSucceeded: resourceManager.dataUrl = dataUrl;
        onChooseCharacterSucceeded: {
            // Connect to chat and game servers
            chatClient.connect(chatServerHost, chatServerPort);
            gameClient.connect(gameServerHost, gameServerPort);
        }
    }
    Mana.ChatClient {
        id: chatClient
        onConnected: authenticate(accountClient.token);
    }
    Mana.GameClient {
        id: gameClient
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
}
