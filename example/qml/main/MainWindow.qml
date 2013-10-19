import QtQuick 2.0
import QtQuick.Window 2.0
import Mana 1.0

Image {
    id: window

    state: ""

    source: "images/login_background.png"
    smooth: false
    fillMode: Image.PreserveAspectCrop
    focus: true;

    function initialize() {
        if (customServer === "") {
            state = "serverSelect";
        } else {
            serverName = customServer;
            client.connect(customServer, customPort);
        }
    }

    readonly property real gameScale: Math.ceil(Math.max(width / 959, height / 959))
    readonly property real backgroundScale: Math.max(width / sourceSize.width,
                                                     height / sourceSize.height);

    property bool characterChosen: false

    ConnectionStatus {
        z: 1;
        anchors.top: parent.top;
        anchors.right: parent.right;
    }

    function switchToChooseCharacter() {
        if (client.accountClient.numberOfCharacters > 0)
            state = "chooseCharacter";
        else
            state = "createCharacter";
    }

    Connections {
        target: client
        onLoggedInChanged: {
            if (client.loggedIn) {
                if (resourceManager.pathsLoaded)
                    switchToChooseCharacter()
                else
                    state = "loadingPaths";
            }
        }
        onReconnectFailed: initialize()
    }
    Connections {
        target: accountClient
        onConnected: {
            if (!client.reconnecting)
                state = "login";
        }
        onLoggedOut: {
            if (state === "game")
                accountClient.disconnect();
        }
    }

    Connections {
        target: resourceManager;
        onPathsLoadedChanged: {
            if (resourceManager.pathsLoaded && state == "loadingPaths")
                switchToChooseCharacter();
        }
    }

    ProgressIndicator {
        anchors.right: window.right
        anchors.bottom: window.bottom
        anchors.margins: 10
        running: accountClient.state === AccountClient.HostLookup ||
                 accountClient.state === AccountClient.Connecting ||
                 loggingIn ||
                 (characterChosen && window.state != "game");
        z: 1;
    }

    property Item currentPage;

    function gotoPage(component) {
        var properties = {
            "width": Qt.binding(function() { return window.width; }),
            "height": Qt.binding(function() { return window.height; }),
            "opacity": 0,
        };
        var newPage = component.createObject(window, properties);

        if (currentPage) {
            properties = { "target": currentPage }
            removePageAnimation.createObject(window, properties).start();
        }

        properties = { "target": newPage }
        showPageAnimation.createObject(window, properties).start();

        currentPage = newPage;
    }

    Component.onCompleted: initialize();

    Component { id: serverPage; ServerPage {} }
    Component { id: loginPage; LoginPage {} }
    Component {
        id: loadingPathsPage;
        Item {
            Text { text: "Loading paths..."; anchors.centerIn: parent }
            Component.onCompleted: window.forceActiveFocus();
        }
    }
    Component { id: characterPage; CharacterPage {} }
    Component { id: newCharacterPage; NewCharacterPage {} }
    Component { id: gamePage; GamePage {} }

    Component {
        id: removePageAnimation;

        SequentialAnimation {
            id: animation;
            property variant target;

            ParallelAnimation {
                PropertyAnimation {
                    target: animation.target;
                    property: "opacity";
                    to: 0;
                    easing.type: Easing.OutQuad;
                }
                PropertyAnimation {
                    target: animation.target;
                    property: "scale";
                    to: 0.9;
                    easing.type: Easing.OutQuad;
                }
            }
            ScriptAction {
                script: animation.target.destroy();
            }
        }
    }

    Component {
        id: showPageAnimation;

        ParallelAnimation {
            id: animation;
            property variant target;

            PropertyAnimation {
                target: animation.target;
                property: "opacity";
                from: 0; to: 1;
                easing.type: Easing.OutQuad;
            }
            PropertyAnimation {
                target: animation.target;
                property: "scale";
                from: 1.1; to: 1;
                easing.type: Easing.OutQuad;
            }
        }
    }

    function back() {
        if (state === "loadingPaths" || state === "chooseCharacter") {
            accountClient.logout();
            state = "login";
            return true;
        } else if (state === "createCharacter") {
            state = "chooseCharacter";
            return true;
        } else if (state === "game") {
            gameClient.leave();
            chatClient.leave();
            characterChosen = false;
            state = "chooseCharacter";
            return true;
        }

        return false;
    }

    Keys.onReturnPressed: {
        if (event.modifiers & Qt.AltModifier) {
            if (client.visibility === Window.FullScreen)
                client.visibility = Window.AutomaticVisibility;
            else
                client.visibility = Window.FullScreen;
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_F10)
            resourcesWindowLoader.active = !resourcesWindowLoader.active;
        else if (event.key === Qt.Key_Backspace)
            event.accepted = back();
    }
    Keys.onReleased: {
        if (event.key === Qt.Key_Back)
            event.accepted = back();
    }

    Component {
        id: resourcesWindowComponent
        ResourcesWindow {}
    }
    Loader {
        id: resourcesWindowLoader
        sourceComponent: resourcesWindowComponent
        active: false;
    }

    states: [
        State {
            name: "serverSelect"
            StateChangeScript {
                script: gotoPage(serverPage);
            }
        },
        State {
            name: "login"
            StateChangeScript {
                script: gotoPage(loginPage);
            }
        },
        State {
            name: "loadingPaths"
            StateChangeScript {
                script: gotoPage(loadingPathsPage);
            }
        },
        State {
            name: "chooseCharacter"
            StateChangeScript {
                script: gotoPage(characterPage);
            }
        },
        State {
            name: "createCharacter"
            StateChangeScript {
                script: gotoPage(newCharacterPage);
            }
        },
        State {
            name: "game"
            when: chatClient.authenticated && gameClient.authenticated
            StateChangeScript {
                script: {
                    accountClient.logout();
                    gotoPage(gamePage);
                }
            }
            PropertyChanges {
                target: window
                source: "";
            }
        }
    ]

    transitions: [
        Transition {
            to: "game"
            PauseAnimation { duration: 500 }
            PropertyAction { property: "source"; }
        }
    ]
}
