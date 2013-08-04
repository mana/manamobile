import QtQuick 2.0
import Mana 1.0

/**
 * A chat message appearing above a player's head.
 */
Item {
    property alias text: chat.text;

    Rectangle {
        anchors.fill: chat;
        anchors.margins: -4;
        radius: 10;
        color: Qt.rgba(0, 0, 0, 0.2);
        opacity: chat.opacity;
    }
    TextShadow { target: chat; }
    Text {
        id: chat;
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;
        color: "white";
        opacity: 0;
        font.pixelSize: 15;

        onTextChanged: {
            opacity = 1;
            chatAnimation.restart();
        }

        SequentialAnimation {
            id: chatAnimation;
            PauseAnimation { duration: Math.min(10000, 2500 + chat.text.length * 50); }
            NumberAnimation { target: chat; property: "opacity"; to: 0; }
        }
    }
}
