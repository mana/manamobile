import QtQuick 2.0

Item {
    id: inventoryPanel;

    BorderImage {
        anchors.fill: parent
        anchors.leftMargin: -22;
        anchors.rightMargin: -33;

        source: "images/scroll_medium_horizontal.png"
        border.left: 40; border.top: 31
        border.right: 38; border.bottom: 32
        smooth: false
    }

    state: "closed";

    width: parent.width / 2 - 22 - 32;
    x: parent.width;

    anchors.top: parent.top;
    anchors.bottom: parent.bottom;

    function toggle() {
        state = state === "closed" ? "open" : "closed";
    }

    Item {
        id: contents
        anchors.fill: parent
        anchors.topMargin: 12
        anchors.bottomMargin: 7
        anchors.leftMargin: 9
        anchors.rightMargin: 5
        clip: true

        ListView {
            anchors.fill: parent;
            leftMargin: 5
            topMargin: 5
            bottomMargin: 5
            rightMargin: 5

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
                    smooth: false
                }

                Text {
                    x: itemGraphic.width;
                    text: info.name;
                    font.pixelSize: 12
                }

                Text {
                    text: model.item.equipmentSlot;
                    font.pixelSize: 12
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
        }
    }

    MouseArea {
        id: handle;

        width: 20;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.left;

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

        Text {
            text: qsTr("Inventory");
            color: "#3f2b25";
            rotation: -90;
            font.bold: true;
            font.pixelSize: 12
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
