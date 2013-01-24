import QtQuick 1.1
import Mana 1.0

Item {
    id: spriteContainer;
    property alias sprites: repeater.model;
    property int direction;
    property variant action;

    Repeater {
        id: repeater;
        model: spriteContainer.sprites;
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
