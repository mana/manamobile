import QtQuick 2.0
import Mana 1.0

Item {
    id: healthBar;

    height: frame.height;
    width: 200;

    BorderImage {
        id: frame;
        anchors.left: parent.left;
        anchors.right: parent.right;

        source: "images/simpleframe.png"
        border.left: 5; border.top: 5;
        border.right: 5; border.bottom: 5;
        smooth: false;
    }
    Rectangle {
        anchors.left: frame.left;
        anchors.top: frame.top;
        anchors.bottom: frame.bottom;
        anchors.margins: 4;
        color: "#12c421";
        width: {
            var health = playerAttributes.health;
            var maxHealth = playerAttributes.maxHealth;
            if (maxHealth <= 0)
                return 0;
            else
                return (health / maxHealth) * (frame.width - 8);
        }

        Behavior on width { NumberAnimation { easing.type: Easing.OutQuad; } }
    }
}
