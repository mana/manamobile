import QtQuick 2.0

MouseArea {
    id: rightPanel

    state: "closed"

    property string page: "inventory"

    property bool toggleOnClick: false

    readonly property bool partlyVisible: x < parent.width

    function toggle(_page) {
        if (state == "closed") {
            page = _page;
            state = "open";
        } else if (page !== _page) {
            page = _page;
        } else {
            state = "closed";
        }
    }

    function openOrClose() {
        var open = rightPanel.x < rightPanel.parent.width - rightPanel.width / 2;
        rightPanel.state = ""  // hack to make sure to trigger transition
        rightPanel.state = open ? "open" : "closed";
    }

    drag.target: rightPanel
    drag.axis: Drag.XAxis
    drag.minimumX: rightPanel.parent.width - rightPanel.width
    drag.maximumX: rightPanel.parent.width
    drag.filterChildren: true
    onReleased: openOrClose();

    Image {
        id: inventoryTab
        source: "images/tab.png"
        smooth: false

        anchors.right: parent.left
        anchors.rightMargin: -4
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height / 6
        z: page == "inventory" ? 1 : 0

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

            onPressed: {
                toggleOnClick = rightPanel.state == "closed" || page == "inventory";
                page = "inventory";
            }
            onClicked: if (toggleOnClick) toggle("inventory");
            onReleased: openOrClose();
        }
    }

    Image {
        id: questTab
        source: "images/tab.png"
        smooth: false

        anchors.right: inventoryTab.right
        anchors.top: inventoryTab.bottom
        anchors.topMargin: 10
        z: page == "quest" ? 1 : 0

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

            onPressed: {
                toggleOnClick = rightPanel.state == "closed" || page == "quest";
                page = "quest";
            }
            onClicked: if (toggleOnClick) toggle("quest");
            onReleased: {
                var open = rightPanel.x < rightPanel.parent.width - rightPanel.width / 2;
                rightPanel.state = ""  // hack to make sure to trigger transition
                rightPanel.state = open ? "open" : "closed";
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
        visible: partlyVisible
    }

    Item {
        id: contents

        anchors.fill: parent
        anchors.topMargin: 12
        anchors.bottomMargin: 7
        anchors.leftMargin: 26

        visible: partlyVisible

        InventoryPanel {
            id: inventoryPanel
            visible: page == "inventory"
        }
        QuestPanel {
            id: questPanel
            visible: page == "quest"
        }
    }

    ScrollTitle {
        text: page == "quest" ? qsTr("Quests") : qsTr("Inventory")
        anchors.horizontalCenterOffset: 14
        visible: partlyVisible
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
