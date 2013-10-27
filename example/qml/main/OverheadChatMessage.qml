import QtQuick 2.0
import Mana 1.0

/**
 * A chat message appearing above a player's head.
 */
Item {
    id: message;

    function showText(text) {
        chat.text = text;
        opacity = 1;
        chatAnimation.restart();
    }

    opacity: 0;

    Rectangle {
        anchors.fill: chat;
        anchors.margins: -4;
        radius: 10;
        color: Qt.rgba(0, 0, 0, 0.2);
    }
    TextShadow { target: chat; }
    Text {
        id: chat;
        textFormat: Text.PlainText
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;
        color: "white";
        font.pixelSize: 15;
    }

    SequentialAnimation {
        id: chatAnimation;
        PauseAnimation { duration: Math.min(10000, 2500 + chat.text.length * 50); }
        NumberAnimation { target: message; property: "opacity"; to: 0; }
    }
}
