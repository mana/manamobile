import QtQuick 2.0
import Mana 1.0

BorderImage {
    visible: gameClient.npcState !== GameClient.NoNpc;

    source: "images/scroll_thin.png";
    border.left: 33; border.top: 33;
    border.right: 34; border.bottom: 27;
    smooth: false;

    Text {
        id: message;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: 10;
        wrapMode: TextEdit.WordWrap;
        text: gameClient.npcMessage;
        font.pixelSize: 12;
    }

    MouseArea {
        anchors.fill: parent;

        onClicked: {
            if (gameClient.npcState === GameClient.NpcAwaitNext)
                gameClient.nextNpcMessage();
        }
    }

    ListView {
        id: choices;
        anchors.top: message.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.margins: 10;

        clip: true;

        model: gameClient.npcChoices;
        visible: gameClient.npcState === GameClient.NpcAwaitChoice;

        delegate: Item {
            width: parent.width;
            height: choice.height;
            Text {
                id: choice;
                color: mouseArea.containsMouse ? "white" : "black";
                wrapMode:Text.WordWrap;
                width: parent.width;
                text: "> " + modelData;
                font.pixelSize: 12;
            }
            MouseArea {
                id: mouseArea;
                anchors.fill: parent;
                hoverEnabled: true;
                onClicked: gameClient.chooseNpcOption(model.index + 1);
            }
        }
    }

    Row {
        anchors.top: message.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;

        height: 32;

        visible: gameClient.npcState === GameClient.NpcAwaitNumberInput;

        LineEdit {
            id: numberInput
            width: parent.width * 0.45;
            height: parent.height;
            inputMethodHints: Qt.ImhDigitsOnly;

            Connections {
                target: gameClient;
                onNpcStateChanged: {
                    if (gameClient.npcState === GameClient.NpcAwaitNumberInput) {
                        numberInput.text = gameClient.npcDefaultNumber;
                    }
                }
            }
        }

        Button {
            width: parent.width * 0.25;
            height: parent.height;
            text: "Submit";
            onClicked: {
                var requireSecondClick = false;
                var value = parseInt(numberInput.text.toLocaleLowerCase());
                if (value > gameClient.npcMaximumNumber) {
                    numberInput.text = gameClient.npcMaximumNumber;
                    requireSecondClick = true;
                }
                if (value < gameClient.npcMinimumNumber) {
                    numberInput.text = gameClient.npcMinimumNumber;
                    requireSecondClick = true;
                }

                if (!requireSecondClick) {
                    gameClient.sendNpcNumberInput(value);
                    numberInput.text = "";
                }
            }
        }
    }
}
