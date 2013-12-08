import QtQuick 2.0
import Mana 1.0

Item {
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

    BorderImage {
        anchors.fill: parent
        anchors.leftMargin: -9
        anchors.rightMargin: -9
        border.bottom: 11
        border.top: 8
        border.right: 17
        border.left: 17
        source: "images/bottom_frame.png"
        smooth: false
    }
    ListView {
        clip: true
        id: chatView
        model: chatModel
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        topMargin: 3
        bottomMargin: 5
        verticalLayoutDirection: ListView.BottomToTop
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
