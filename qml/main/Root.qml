import Qt 4.7
import Mana 1.0 as Mana

Rectangle {
    width: 800
    height: 480

    // Connect to the default server
    Component.onCompleted: {
        accountClient.connect("testing.manasource.org", 9601)
    }

    Mana.AccountClient {
        id: accountClient

        onConnected: requestRegistrationInfo();
        onLoginSucceeded: {
            resourceManager.dataUrl = dataUrl;
        }
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

    MainWindow {
        anchors.fill: parent
    }
}
