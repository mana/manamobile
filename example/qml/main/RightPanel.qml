import QtQuick 2.0

Item {
    id: rightPanel

    state: "closed"

    function toggle(page) {
        state = state === "closed" ? "open" : "closed";
    }

    Column {
        anchors.right: parent.left
        anchors.rightMargin: -3
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height / 6
        spacing: 10

        Image {
            id: inventoryTab
            source: "images/tab.png"
            smooth: false

            Image {
                source: "images/tab_icon_inventory.png"
                anchors.centerIn: parent
                smooth: false
            }
            MouseArea {
                anchors.fill: parent
                anchors.margins: -5

                drag.target: rightPanel
                drag.axis: Drag.XAxis
                drag.minimumX: rightPanel.parent.width - rightPanel.width
                drag.maximumX: rightPanel.parent.width

                onClicked: {
                    if (rightPanel.state === "closed" || inventoryPanel.visible)
                        toggle();

                    inventoryPanel.visible = true
                    questPanel.visible = false
                }
                onReleased: {
                    var open = rightPanel.x < rightPanel.parent.width - rightPanel.width / 2;
                    rightPanel.state = ""  // hack to make sure to trigger transition
                    rightPanel.state = open ? "open" : "closed";
                }
            }
        }

        Image {
            id: questTab
            source: "images/tab.png"
            anchors.rightMargin: -3
            smooth: false

            Image {
                source: "images/tab_icon_questlog.png"
                anchors.centerIn: parent
                smooth: false
            }
            MouseArea {
                anchors.fill: parent
                anchors.margins: -5

                drag.target: rightPanel
                drag.axis: Drag.XAxis
                drag.minimumX: rightPanel.parent.width - rightPanel.width
                drag.maximumX: rightPanel.parent.width

                onClicked: {
                    if (rightPanel.state === "closed" || questPanel.visible)
                        toggle();

                    inventoryPanel.visible = false
                    questPanel.visible = true
                }
                onReleased: {
                    var open = rightPanel.x < rightPanel.parent.width - rightPanel.width / 2;
                    rightPanel.state = ""  // hack to make sure to trigger transition
                    rightPanel.state = open ? "open" : "closed";
                }
            }
        }
    }

    BorderImage {
        anchors.fill: parent
        anchors.leftMargin: -1
        anchors.rightMargin: -33

        source: "images/scroll_medium_horizontal.png"
        border.left: 40; border.top: 31
        border.right: 38; border.bottom: 32
        smooth: false
    }

    Item {
        id: contents

        anchors.fill: parent
        anchors.topMargin: 12
        anchors.bottomMargin: 7
        anchors.leftMargin: 26

        clip: true

        InventoryPanel { id: inventoryPanel }
        QuestPanel { id: questPanel; visible: false }
    }

    states: [
        State {
            name: "open";
            PropertyChanges {
                target: rightPanel
                x: rightPanel.parent.width - rightPanel.width
            }
        },
        State {
            name: "closed";
            PropertyChanges {
                target: rightPanel
                x: rightPanel.parent.width
            }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { property: "x"; easing.type: Easing.OutQuad }
        }
    ]
}
