import QtQuick 2.0

ListView {
    anchors.fill: parent
    anchors.leftMargin: 5
    anchors.rightMargin: 5

    topMargin: 5
    bottomMargin: 5

    model: itemDB.isLoaded ? gameClient.inventoryListModel : null

    delegate: Item {
        anchors.left: parent.left
        anchors.right: parent.right

        property variant info: itemDB.getInfo(model.item.id)

        height: 36

        Rectangle {
            anchors.fill: parent
            anchors.margins: 1

            color: Qt.rgba(0, 0, 0, 0.2)
            border.color: Qt.rgba(0, 0, 0, 0.4)
            visible: item.isEquipped
        }

        Image {
            id: itemGraphic
            x: 2; y: 2
            // TODO: use imageprovider for this + handling dye
            source: resourceManager.dataUrl + resourceManager.itemIconsPrefix + info.image
            smooth: false
        }

        Text {
            text: info.name;

            anchors.left: parent.left
            anchors.leftMargin: 2 + 32 + 5
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }

        Text {
            text: item.amount
            visible: item.amount > 1

            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            font.pixelSize: 12
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                if (model.item.isEquipped)
                    gameClient.unequip(model.item.slot);
                else
                    gameClient.equip(model.item.slot);
            }
        }
    }
}
