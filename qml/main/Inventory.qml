import QtQuick 1.1

Item {
    focus: window.state == "inventoryPage";

    ListView {
        id: inventoryList;
        focus: window.state == "chooseCharacter";

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
                source: resourceManager.dataUrl +
                        resourceManager.itemIconsPrefix + info.image;
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

    Keys.onPressed: {
        if (event.key == Qt.Key_I)
            gamePage.state = "game";
        event.accepted = true;
    }
}
