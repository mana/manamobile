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

    /* This object is expected to be created before the attributes arrive from
     * the game server. When the attributes are set for the first time, these
     * bindings are set up.
     */
    property QtObject connections: Connections {
        target: gameClient.attributeListModel;
        onAttributeAdded: {
            switch (id) {
            case 13: playerAttributes.health = binding(attribute); break;
            case 14: playerAttributes.maxHealth = binding(attribute); break;
            case 18: playerAttributes.gold = binding(attribute); break;
            }
        }
    }

    function binding(attribute) {
        return Qt.binding(function() { return attribute.modified; });
    }
}
