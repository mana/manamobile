import QtQuick 2.0
import Mana 1.0

Rectangle {
    visible: gameClient.npcState !== GameClient.NoNpc;

    radius: 10;
    color: "grey";

    Text {
        id: message;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: 10;
        wrapMode: TextEdit.WordWrap;
        text: gameClient.npcMessage;
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
            }
            MouseArea {
                id: mouseArea;
                anchors.fill: parent;
                hoverEnabled: true;
                onClicked: gameClient.chooseNpcOption(model.index + 1);
            }
        }
    }
}
