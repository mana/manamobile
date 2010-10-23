import Qt 4.7
import Mana 1.0 as Mana

Rectangle {
    width: 800
    height: 480

    Mana.AccountClient {
        id: accountClient

        onLoginSucceeded: {
            resourceManager.dataUrl = accountClient.dataUrl;
        }
        onChooseCharacterSucceeded: {
            // Connect to chat and game servers
            chatClient.connect(chatServerHost, chatServerPort);
            gameClient.connect(gameServerHost, gameServerPort);
        }
    }
    Mana.ChatClient {
        id: chatClient
        onConnected: sendToken(accountClient.token);
    }
    Mana.GameClient {
        id: gameClient
        onConnected: sendToken(accountClient.token);
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
