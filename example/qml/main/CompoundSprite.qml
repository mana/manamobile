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

            direction: spriteContainer.direction;
            action: spriteContainer.action;
            z: model.slot;
            spriteReference: model.sprite;

            anchors.horizontalCenter: spriteContainer.horizontalCenter;
            anchors.bottom: spriteContainer.bottom;
        }
    }
}
