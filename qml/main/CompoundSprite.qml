import QtQuick 2.0
import Mana 1.0

Item {
    id: spriteContainer;

    property alias sprites: repeater.model;

    property int direction;
    property string action;

    property int maxHeight: childrenRect.height;

    Repeater {
        id: repeater;

        delegate: Sprite {
            id: element;
            parent: spriteContainer;

            direction: spriteContainer.direction;
            action: spriteContainer.action;
            z: model.slot;
            spriteReference: model.sprite;

            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: parent.bottom;
        }
    }
}
