import QtQuick 2.0
import QtQuick.Layouts 1.0

MouseArea {
    id: statusPanel;

    state: "closed"

    function toggle() {
        state = state === "closed" ? "open" : "closed";
    }

    function limitPrecision(number, precision) {
        var p = Math.pow(10, precision);
        return Math.round(number * p) / p;
    }

    function openOrClose() {
        var open = -statusPanel.x < statusPanel.width / 2;
        statusPanel.state = ""  // hack to make sure to trigger transition
        statusPanel.state = open ? "open" : "closed";
    }

    drag.target: statusPanel;
    drag.axis: Drag.XAxis;
    drag.minimumX: -statusPanel.width;
    drag.maximumX: 0;
    drag.filterChildren: true
    onReleased: openOrClose();

    Image {
        id: tab
        source: "images/tab.png"
        y: parent.height / 4
        anchors.left: parent.right
        anchors.leftMargin: -3
        smooth: false
    }
    Image {
        source: "images/tab_icon_character.png"
        anchors.centerIn: tab
        smooth: false
    }
    MouseArea {
        id: handle;

        anchors.fill: tab
        anchors.margins: -5

        drag.target: statusPanel;
        drag.axis: Drag.XAxis;
        drag.minimumX: -statusPanel.width;
        drag.maximumX: 0;

        onClicked: toggle();
        onReleased: openOrClose();
    }

    BorderImage {
        anchors.fill: parent
        anchors.leftMargin: -33;
        anchors.rightMargin: -1

        source: "images/scroll_medium_horizontal.png"
        border.left: 40; border.top: 31
        border.right: 38; border.bottom: 32
        smooth: false
    }

    Item {
        id: contents

        anchors.fill: parent
        anchors.topMargin: 12
        anchors.rightMargin: 28
        anchors.bottomMargin: 7

        clip: flickable.interactive

        Flickable {
            id: flickable

            anchors.top: titleBackground.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 8

            interactive: contentHeight > height
            contentHeight: grid.height

            GridLayout {
                id: grid

                anchors.left: parent.left
                anchors.right: parent.right

                columns: 2
                rowSpacing: 5
                columnSpacing: 5

                AttributeLabel {
                    name: qsTr("Strength")
                    value: playerAttributes.strength
                    Layout.row: 0
                    Layout.column: 0
                    Layout.fillWidth: true
                }
                AttributeLabel {
                    name: qsTr("Damage")
                    value: {
                        var base = playerAttributes.damage;
                        var min = Math.round(base);
                        var max = Math.round(base + playerAttributes.damageDelta);
                        return min + "-" + max;
                    }
                    Layout.row: 0
                    Layout.column: 1
                    Layout.fillWidth: true
                }

                AttributeLabel {
                    name: qsTr("Agility")
                    value: playerAttributes.agility
                    Layout.row: 1
                    Layout.column: 0
                    Layout.fillWidth: true
                }
                AttributeLabel {
                    name: qsTr("Movement speed")
                    value: limitPrecision(playerAttributes.movementSpeed, 1)
                    Layout.row: 1
                    Layout.column: 1
                    Layout.fillWidth: true
                }
                AttributeLabel {
                    name: qsTr("Dodge")
                    value: limitPrecision(playerAttributes.dodge, 1)
                    Layout.row: 2
                    Layout.column: 1
                    Layout.fillWidth: true
                }

                AttributeLabel {
                    name: qsTr("Dexterity")
                    value: playerAttributes.dexterity
                    Layout.row: 3
                    Layout.column: 0
                    Layout.fillWidth: true
                }
                AttributeLabel {
                    name: qsTr("Hit chance")
                    value: limitPrecision(playerAttributes.hitChance, 1)
                    Layout.row: 3
                    Layout.column: 1
                    Layout.fillWidth: true
                }

                AttributeLabel {
                    name: qsTr("Vitality")
                    value: playerAttributes.vitality
                    Layout.row: 4
                    Layout.column: 0
                    Layout.fillWidth: true
                }
                AttributeLabel {
                    name: qsTr("Health")
                    value: playerAttributes.maxHealth
                    Layout.row: 4
                    Layout.column: 1
                    Layout.fillWidth: true
                }
                AttributeLabel {
                    name: qsTr("Regeneration")
                    value: limitPrecision(playerAttributes.healthRegeneration / 10, 2) + "/s"
                    Layout.row: 5
                    Layout.column: 1
                    Layout.fillWidth: true
                }
                AttributeLabel {
                    name: qsTr("Defense")
                    value: limitPrecision(playerAttributes.defense, 1)
                    Layout.row: 6
                    Layout.column: 1
                    Layout.fillWidth: true
                }

                AttributeLabel {
                    name: qsTr("Intelligence")
                    value: playerAttributes.intelligence
                    Layout.row: 7
                    Layout.column: 0
                    Layout.fillWidth: true
                }

                AttributeLabel {
                    name: qsTr("Willpower")
                    value: playerAttributes.willpower
                    Layout.row: 8
                    Layout.column: 0
                    Layout.fillWidth: true
                }
            }
        }

        Image {
            id: titleBackground
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            source: "images/header_ornamental.png"
            fillMode: Image.TileHorizontally
            horizontalAlignment: Image.AlignLeft
            smooth: false
        }

        Text {
            id: nameLabel
            text: gameClient.playerName
            font.pixelSize: 12
            font.bold: true
            anchors.left: parent.left
            anchors.baseline: levelLabel.baseline
            anchors.leftMargin: 8
            color: "#3F2B25"
        }

        Text {
            id: levelLabel
            anchors.bottom: titleBackground.bottom
            anchors.right: parent.right
            anchors.rightMargin: 8
            text: 'Level <font size="+1">' + Math.floor(playerAttributes.level) + "</font>"
            font.pixelSize: 12
            color: "#3F2B25"
        }

/*
  // TODO: This would be useful as a debug window

        ListView {
            anchors.fill: parent;
            anchors.leftMargin: 5
            anchors.rightMargin: 5

            topMargin: 5
            bottomMargin: 5

            model: attributeDB.isLoaded ? gameClient.attributeListModel : null;
            delegate: Item {
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: attributeName.height;

                Text {
                    id: attributeName;
                    text: {
                        var attributeInfo = attributeDB.getInfo(model.value.attributeId);
                        return attributeInfo.name + " " + model.value.modified;
                    }
                    font.pixelSize: 12
                }
                Text {
                    property real diff: model.value.modified - model.value.base;
                    text: diff > 0 ? "(+" + diff + ")" : "(" + diff + ")";
                    visible: diff != 0;
                    color: diff > 0 ? "green" : "red";
                    font.pixelSize: 12
                    anchors.left: parent.right;
                    anchors.leftMargin: -50;
                }
            }
        }
*/
    }

    states: [
        State {
            name: "open";

            PropertyChanges {
                target: statusPanel;
                x: 0;
            }
        },
        State {
            name: "closed";
            PropertyChanges {
                target: statusPanel;
                x: -statusPanel.width;
            }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { property: "x"; easing.type: Easing.OutQuad }
        }
    ]
}
