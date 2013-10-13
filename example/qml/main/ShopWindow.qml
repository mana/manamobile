import QtQuick 2.0
import Mana 1.0

Rectangle {
    id: shopWindow
    visible: false

    width: 200
    height: 200

    Connections {
        target: gameClient
        onShopOpened: {
            console.log("shopOpened", mode);
            shopWindow.visible = true;
        }
    }

    Text {
        id: titleLabel
        anchors.horizontalCenter: parent.horizontalCenter
        text: {
            switch (gameClient.shopMode) {
            case GameClient.BuyFromShop:
                return "Buy";
            case GameClient.SellToShop:
                return "Sell";
            }
            return "";
        }
    }

    ListView {
        anchors.top: titleLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: closeButton.top
        anchors.bottomMargin: 10

        model: itemDB.isLoaded ? gameClient.shopListModel : null
        delegate: Item {
            anchors.left: parent.left
            anchors.right: parent.right

            property variant info: itemDB.getInfo(model.itemId)

            height: 36

            Image {
                id: itemGraphic
                x: 2; y: 2
                // TODO: use imageprovider for this + handling dye
                source: resourceManager.dataUrl + resourceManager.itemIconsPrefix + info.image
                smooth: false
            }

            Text {
                text: info.name

                anchors.left: parent.left
                anchors.leftMargin: 2 + 32 + 5
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            Text {
                text: amount
                visible: amount > 0

                anchors.right: parent.right
                anchors.rightMargin: 4
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
                font.pixelSize: 12
            }

            MouseArea {
                anchors.fill: parent
                onClicked: gameClient.buySell(model.itemId, 1);
            }
        }
    }

    Button {
        id: closeButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: qsTr("Close")
        onClicked: shopWindow.visible = false;
    }
}
