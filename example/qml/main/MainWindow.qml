import QtQuick 2.0
import Mana 1.0

Image {
    id: window

    state: "serverSelect"

    source: "images/login_background.png"
    smooth: false
    fillMode: Image.PreserveAspectCrop
    focus: true;

    readonly property real backgroundScale: Math.max(width / sourceSize.width,
                                                     height / sourceSize.height);

    property bool loggingIn: false
    property bool loggedIn: false
    property bool characterChosen: false

    function authenticated() {
        loggedIn = true
        loggingIn = false
        state = "loadingPaths";
    }

    ConnectionStatus {
        z: 1;
        anchors.top: parent.top;
        anchors.right: parent.right;
    }

    Connections {
        target: accountClient;

        onLoginSucceeded: authenticated();
        onRegistrationSucceeded: authenticated();
        onLoginFailed: loggingIn = false;
        onRegistrationFailed: loggingIn = false;
    }
    Connections {
        target: resourceManager;
        onPathsLoadedChanged: {
            if (resourceManager.pathsLoaded && state == "loadingPaths")
                state = "chooseCharacter";
        }
    }

    ProgressIndicator {
        anchors.right: window.right
        anchors.bottom: window.bottom
        anchors.margins: 10
        running: accountClient.state === AccountClient.Connecting || loggingIn
                 || (characterChosen && window.state != "game");
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

    Component.onCompleted: {
        if (customServer === "") {
            gotoPage(serverPage);
        } else {
            serverName = customServer;
            accountClient.connect(customServer, customPort);
        }
    }

    Component { id: serverPage; ServerPage {} }
    Component { id: loginPage; LoginPage {} }
    Component {
        id: loadingPathsPage;
        Item {
            Text { text: "Loading paths..."; anchors.centerIn: parent }
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

    Keys.onPressed: {
        if (event.key === Qt.Key_F10)
            resourcesWindowLoader.active = !resourcesWindowLoader.active;
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
        },
        State {
            name: "login"
            when: accountClient.connected && !characterChosen
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
                script: gotoPage(gamePage);
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
