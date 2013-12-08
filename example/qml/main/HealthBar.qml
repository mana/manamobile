import QtQuick 2.0
import Mana 1.0

Item {
    height: frame.height
    width: 200

    property real levelProgress: {
        var level = playerAttributes.level;
        return level - Math.floor(level);
    }

    property real healthRatio: {
        var health = playerAttributes.health;
        var maxHealth = playerAttributes.maxHealth;
        if (maxHealth <= 0)
            return 0;
        else
            return health / maxHealth;
    }

    Behavior on levelProgress { NumberAnimation { easing.type: Easing.OutQuad; } }
    Behavior on healthRatio { NumberAnimation { easing.type: Easing.OutQuad; } }

    BorderImage {
        id: frame
        anchors.left: parent.left
        anchors.right: parent.right

        source: "images/top_bar.png"
        border.left: 16
        border.right: 16
        smooth: false
    }

    Rectangle {
        id: levelProgressBar

        x: 4; y: 2
        width: levelProgress * (frame.width - 8);
        height: 3
        color: "#f5deb0"
    }

    Rectangle {
        id: healthBar

        x: 14; y: 9
        width: healthRatio * (frame.width - 28)
        height: 6
        color: "#12c421"
    }
}
