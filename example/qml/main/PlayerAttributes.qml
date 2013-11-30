import QtQuick 2.0
import Mana 1.0

/**
 * Provides access to some attributes for display in the UI.
 */
QtObject {
    id: playerAttributes

    // The 6 main attributes that the player can upgrade
    property real strength: 0
    property real agility: 0
    property real vitality: 0
    property real intelligence: 0
    property real dexterity: 0
    property real willpower: 0

    property real health: 0
    property real maxHealth: 0              // vitality
    property real healthRegeneration: 0     // vitality
    property real damage: 0                 // strength
    property real damageDelta: 0
    property real hitChance: 0              // dexterity (via accuracy)
    property real capacity: 0               // strength
    property real dodge: 0                  // agility
    property real magicDodge: 0             // -
    property real movementSpeed: 0          // agility
    property real accuracy: 0               // dexterity
    property real defense: 0                // vitality
    property real magicDefense: 0           // -

    property real gold: 0

    property real xp: 0
    property real level: 0

    property real soldierReputation: 0
    property real rebelReputation: 0

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
        case  1: playerAttributes.strength      = binding(attribute); break;
        case  2: playerAttributes.agility       = binding(attribute); break;
        case  3: playerAttributes.vitality      = binding(attribute); break;
        case  4: playerAttributes.intelligence  = binding(attribute); break;
        case  5: playerAttributes.dexterity     = binding(attribute); break;
        case  6: playerAttributes.willpower     = binding(attribute); break;

        case  7: playerAttributes.accuracy      = binding(attribute); break;
        case  8: playerAttributes.defense       = binding(attribute); break;
        case  9: playerAttributes.dodge         = binding(attribute); break;
        case 10: playerAttributes.magicDodge    = binding(attribute); break;
        case 11: playerAttributes.magicDefense  = binding(attribute); break;
//        case 12: break; // Bonus att. speed
        case 13: playerAttributes.health        = binding(attribute); break;
        case 14: playerAttributes.maxHealth     = binding(attribute); break;
        case 15: playerAttributes.healthRegeneration = binding(attribute); break;
        case 16: playerAttributes.movementSpeed = binding(attribute); break;
//        case 17: break;  // Movement speed (raw)
        case 18: playerAttributes.gold          = binding(attribute); break;
        case 19: playerAttributes.capacity      = binding(attribute); break;
        case 20: playerAttributes.xp            = binding(attribute); break;
        case 21: playerAttributes.level         = binding(attribute); break;
//        case 22: playerAttributes.attackSpeed   = binding(attribute); break;
//        case 23: playerAttributes.range         = binding(attribute); break;
        case 24: playerAttributes.damage        = binding(attribute); break;
        case 25: playerAttributes.damageDelta   = binding(attribute); break;
        case 26: playerAttributes.hitChance     = binding(attribute); break;
//        case 27: break; // Attackspeed
        case 28: playerAttributes.soldierReputation = binding(attribute); break;
        case 29: playerAttributes.rebelReputation   = binding(attribute); break;
        }
    }

    function binding(attribute) {
        return Qt.binding(function() { return attribute.modified; });
    }
}
