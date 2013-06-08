import QtQuick 2.0
import Mana 1.0

Rectangle {
    opacity: gameClient.npcDialogManager.expectedInput != NpcDialogManager.EXPECT_NOTHING ? 1 : 0;

    radius: 10;
    color: "grey";

    Text {
        id: message;
        anchors.fill: parent;
        anchors.margins: 10;
        wrapMode: TextEdit.WordWrap;
        text: gameClient.npcDialogManager.currentText;
    }

    MouseArea {
        anchors.fill: parent;

        onClicked: {
            if (gameClient.npcDialogManager.expectedInput == NpcDialogManager.EXPECT_NEXT)
                gameClient.npcDialogManager.next();
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

        model: gameClient.npcDialogManager.currentChoices;
        opacity: gameClient.npcDialogManager.expectedInput == NpcDialogManager.EXPECT_CHOICE ? 1 : 0;

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
                onClicked: {
                    gameClient.npcDialogManager.choose(model.index + 1);
                }
            }
        }
    }

}
