import QtQuick 2.0
import Mana 1.0

FocusScope {
    id: chatBar;

    Keys.onReturnPressed: sayText();
    Keys.onEnterPressed: sayText();

    function open() {
        chatBar.focus = true;
    }

    function sayText() {
        if (chatInput.text != "") {
            gameClient.say(chatInput.text);
            chatInput.text = "";
        }

        chatInput.focus = false;
        Qt.inputMethod.hide();
        gamePage.focus = true;
    }

    LineEdit {
        id: chatInput;
        anchors.left: parent.left;
        anchors.right: sayButton.left;

        states: [
            State {
                name: "opened";
                when: chatBar.activeFocus;
                PropertyChanges {
                    target: chatInput;
                    y: -chatInput.height - 5;
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation {
                        property: "y";
                        easing.type: Easing.InOutQuad;
                    }
                    ScriptAction {
                        script: chatInput.focus = chatInput.y !== 0;
                    }
                }
            }
        ]
    }
    Button {
        id: sayButton;
        anchors.right: parent.right;
        anchors.verticalCenter: chatInput.verticalCenter;
        text: "Say";
        onClicked: chatBar.sayText();
        KeyNavigation.left: chatInput;
    }
}
