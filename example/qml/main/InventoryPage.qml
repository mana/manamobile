import QtQuick 2.0

Rectangle {
    id: inventoryPanel;

    state: "closed";
    color: "BurlyWood";

    width: parent.width / 2;
    x: parent.width;

    anchors.top: parent.top;
    anchors.bottom: parent.bottom;

    function toggle() {
        state = state === "closed" ? "open" : "closed";
    }

    ListView {
        anchors.fill: parent;

        id: inventoryList;

        width: window.width;
        height: window.height;
        model: itemDB.isLoaded ? gameClient.inventoryListModel : null;
        delegate: Item {
            id: itemInfo;
            property variant info: itemDB.getInfo(model.item.id);

            height: 30;

            Image {
                id: itemGraphic;
                // TODO: use imageprovider for this + handling dye
                source: resourceManager.dataUrl + resourceManager.itemIconsPrefix + info.image;
            }

            Text {
                x: itemGraphic.width;
                text: info.name;
            }

            Text {
                text: model.item.equipmentSlot;
            }

            Text {
                color: "red";
                text: model.item.amount > 1 ? model.item.amount : "";
                font.bold: true;
                x: itemGraphic.width / 2;
                y: itemGraphic.height / 2;
            }

            MouseArea {
                height: 30;
                width: 100;

                onDoubleClicked: {
                    if (model.item.equipmentSlot == 0)
                        gameClient.equip(model.item.slot);
                    else
                        gameClient.unequip(model.item.slot);
                }
            }
        }

        highlight: Rectangle {
            color: "black";
            opacity: 0.2;
            width: inventoryList.width;
        }

        onCountChanged: {
            // Select first item when possible
            if (currentIndex == -1 && count > 0)
                currentIndex = 0;
        }

        Rectangle {
            anchors.fill: parent;
            color: "transparent";
            border.color: "black";
        }
    }

    Rectangle {
        width: 20;
        color: "black";

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.left;

        MouseArea {
            id: handle;

            anchors.fill: parent;

            drag.target: inventoryPanel;
            drag.axis: Drag.XAxis;
            drag.minimumX: inventoryPanel.parent.width - inventoryPanel.width;
            drag.maximumX: inventoryPanel.parent.width;

            onClicked: toggle();
            onReleased: {
                var open = inventoryPanel.x < inventoryPanel.parent.width - inventoryPanel.width / 2;
                inventoryPanel.x = open ? inventoryPanel.parent.width - inventoryPanel.width
                                        : inventoryPanel.parent.width;
                inventoryPanel.state = open ? "open" : "closed";
            }
        }

        Text {
            text: qsTr("Inventory");
            color: "white";
            rotation: -90;
            font.bold: true;
            anchors.centerIn: parent;
        }
    }

    Behavior on x {
        NumberAnimation { easing.type: Easing.OutQuad }
    }

    states: [
        State {
            name: "open";
            PropertyChanges {
                target: inventoryPanel;
                x: inventoryPanel.parent.width - inventoryPanel.width;
            }
        },
        State {
            name: "closed";
            PropertyChanges {
                target: inventoryPanel;
                x: inventoryPanel.parent.width;
            }
        }
    ]
}
