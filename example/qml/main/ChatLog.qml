import QtQuick 2.0
import Mana 1.0

Item {
    clip: true
    property int maxHeight;
    height: 100

    ListModel { id: chatModel }
    Connections {
        target: gameClient
        onChatMessage: {
            var name = being ? being.name : qsTr("Server");
            chatModel.insert(0, { name: name, message: message });
            chatView.positionViewAtBeginning();
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 0.25)
    }
    ListView {
        id: chatView
        model: chatModel
        anchors.fill: parent
        verticalLayoutDirection: ListView.BottomToTop
        anchors.margins: 5
        delegate: Item {
            height: messageLabel.height
            anchors.left: parent.left
            anchors.right: parent.right
            TextShadow { target: nameLabel }
            TextShadow { target: messageLabel }
            Text {
                id: nameLabel
                color: "BurlyWood"
                font.bold: true
                font.pixelSize: 12
                text: "<" + model.name + ">"
                textFormat: Text.PlainText
            }
            Text {
                id: messageLabel
                color: "beige"
                font.pixelSize: 12
                text: model.message
                textFormat: Text.PlainText
                wrapMode: Text.Wrap
                anchors.left: nameLabel.right
                anchors.right: parent.right
                anchors.leftMargin: 5
            }
        }
    }
}
