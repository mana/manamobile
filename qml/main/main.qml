import Qt 4.7
import Mana 1.0

Rectangle {
    id: window
    color: "#fffcf2"
    width: 800
    height: 480

    state: "login"

    property bool connecting: false
    property bool loggingIn: false
    property bool loggedIn: false
    property string errorMessage: ""

    Component.onCompleted: {
        connecting = true
        loginManager.connectToLoginServer("dev.themanaworld.org", 9601)
    }

    Connections {
        target: loginManager;

        onLoginSucceeded: {
            print("Logged in!")
            loggedIn = true
            loggingIn = false
            state = "chooseCharacter"
        }
        onLoginFailed: {
            errorMessage = loginManager.error
            loggingIn = false
        }
        onConnectedChanged: {
            if (loginManager.connected)
                window.connecting = false
        }
    }

    Image {
        x: -width * 0.1
        y: parent.height - height * 0.9
        sourceSize: Qt.size(parent.width / 2,
                            parent.width / 2)
        source: "images/mana.svg"
        opacity: 0.5
    }

    ProgressIndicator {
        anchors.right: window.right
        anchors.bottom: window.bottom
        anchors.margins: 10
        running: connecting || loggingIn
    }

    LoginPage {
        id: loginPage
        width: parent.width
        height: parent.height
    }
    CharacterPage {
        id: characterPage
        width: parent.width
        height: parent.height
        opacity: 0
        x: width / 4
    }

    states: [
        State {
            name: "login"
        },
        State {
            name: "chooseCharacter"
            PropertyChanges {
                target: loginPage
                x: -loginPage.width / 4
                opacity: 0
            }
            PropertyChanges {
                target: characterPage
                x: 0
                opacity: 1
            }
        }
    ]

    transitions: [
        Transition {
            SequentialAnimation {
                NumberAnimation {
                    target: loginPage
                    properties: "x,opacity"
                    duration: 500
                    easing.type: Easing.InQuad
                }
                NumberAnimation {
                    target: characterPage
                    properties: "x,opacity"
                    duration: 500
                    easing.type: Easing.OutQuad
                }
            }
        }
    ]
}
