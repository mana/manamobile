import Qt 4.7
import Mana 1.0

Rectangle {
    id: window
    color: "#fffcf2"

    state: "login"

    property bool loggingIn: false
    property bool loggedIn: false
    property bool characterChosen: false

    Connections {
        target: accountClient;

        onLoginSucceeded: {
            print("Logged in!")
            loggedIn = true
            loggingIn = false
            state = "chooseCharacter"
        }
        onLoginFailed: {
            loggingIn = false
        }
    }

    Image {
        x: -width * 0.1
        y: parent.height - height * 0.9
        sourceSize: Qt.size(parent.width / 2,
                            parent.width / 2)
        width: sourceSize.width;
        height: sourceSize.height;
        source: "images/mana.svg"
        opacity: 0.5
    }

    ProgressIndicator {
        anchors.right: window.right
        anchors.bottom: window.bottom
        anchors.margins: 10
        running: accountClient.state == AccountClient.Connecting || loggingIn
                 || (characterChosen && window.state != "game")
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
    GamePage {
        id: gamePage
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
        },
        State {
            name: "game"
            when: chatClient.authenticated && gameClient.authenticated
            extend: "chooseCharacter"
            PropertyChanges {
                target: characterPage
                x: -characterPage.width / 4
                opacity: 0
            }
            PropertyChanges {
                target: gamePage
                x: 0
                opacity: 1
            }
        }
    ]

    transitions: [
        Transition {
            SequentialAnimation {
                NumberAnimation {
                    properties: "x,opacity"
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
        }
    ]
}
