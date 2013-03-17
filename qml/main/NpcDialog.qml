import QtQuick 1.1
import Mana 1.0

Rectangle {
    property variant npc;

    focus: gameClient.npcDialogManager.expectedInput != NpcDialogManager.EXPECT_NOTHING;
    opacity: focus ? 1 : 0;

    radius: 10;
    color: "grey";

    Text {
        anchors.fill: parent;
        anchors.margins: 10;
        text: gameClient.npcDialogManager.currentText;
    }

    MouseArea {
        anchors.fill: parent;

        onClicked: {
            gameClient.npcDialogManager.next();
        }
    }
}
