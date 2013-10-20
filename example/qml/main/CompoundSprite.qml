import QtQuick 2.0
import Mana 1.0

Item {
    id: spriteContainer;

    property QtObject being: null

    property alias sprites: repeater.model;

    property int direction: being ? being.spriteDirection : Action.DIRECTION_DOWN
    property string action: being ? being.action : "stand"

    property int maxHeight: childrenRect.height;

    function restartAction() {
        var spriteCount = repeater.count;
        for (var i = 0; i < spriteCount; ++i)
            repeater.itemAt(i).playAction(action);
    }

    Repeater {
        id: repeater;

        model: being ? being.spriteListModel : null

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
