import QtQuick 2.0
import Mana 1.0

/**
 * Provides access to some attributes for display in the UI.
 */
QtObject {
    id: playerAttributes;

    property real health: 0;
    property real maxHealth: 0;

    property real gold: 0;

    Component.onCompleted: {
        var model = gameClient.attributeListModel;
        for (var i = 0; i < model.length; ++i) {
            var attribute = model.attributeAt(i);
            bindAttribute(attribute.attributeId, attribute);
        }
    }

    /* This object may be created before certain attributes arrive from the
     * game server. Bind those attributes when they appear.
     */
    property QtObject connections: Connections {
        target: gameClient.attributeListModel;
        onAttributeAdded: bindAttribute(id, attribute);
    }

    function bindAttribute(id, attribute) {
        switch (id) {
        case 13: playerAttributes.health = binding(attribute); break;
        case 14: playerAttributes.maxHealth = binding(attribute); break;
        case 18: playerAttributes.gold = binding(attribute); break;
        }
    }

    function binding(attribute) {
        return Qt.binding(function() { return attribute.modified; });
    }
}
