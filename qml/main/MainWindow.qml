import QtQuick 2.0
import Mana 1.0

Rectangle {
    id: window
    color: "#fffcf2"
    focus: true;

    state: "serverSelect"

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

    Image {
        x: -width * 0.1
        y: parent.height - height * 0.9
        sourceSize: Qt.size(parent.width / 2.2,
                            parent.width / 2.2)
        width: sourceSize.width;
        height: sourceSize.height;
        source: "images/mana.svg"
        opacity: 0.5
    }

    ProgressIndicator {
        anchors.right: window.right
        anchors.bottom: window.bottom
        anchors.margins: 10
        running: accountClient.state === AccountClient.Connecting || loggingIn
                 || (characterChosen && window.state != "game");
        z: 1;
    }

    property variant currentPage;

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

    Component.onCompleted: gotoPage(serverPage);

    Component { id: serverPage; ServerPage {} }
    Component { id: loginPage; LoginPage {} }
    Component {
        id: loadingPathsPage;
        Item {
            Text { text: "Loading paths..."; anchors.centerIn: parent }
        }
    }
    Component { id: characterPage; CharacterPage {} }
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
                    from: 1; to: 0;
                    easing.type: Easing.OutQuad;
                }
                PropertyAnimation {
                    target: animation.target;
                    property: "x";
                    from: 0; to: -50;
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
                property: "x";
                from: 50; to: 0;
                easing.type: Easing.OutQuad;
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_F10)
            resourcesWindow.visible = !resourcesWindow.visible;
    }

    ResourcesWindow {
        id: resourcesWindow
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
            name: "game"
            when: chatClient.authenticated && gameClient.authenticated
            extend: "chooseCharacter"
            StateChangeScript {
                script: gotoPage(gamePage);
            }
        }
    ]
}
