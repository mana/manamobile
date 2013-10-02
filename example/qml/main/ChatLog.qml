import QtQuick 2.0
import Mana 1.0

Item {
    clip: true
    property int maxHeight;
    height: Math.min(maxHeight, chatView.childrenRect.height + chatView.anchors.margins * 2);

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
        anchors.left: chatView.left
        anchors.right: chatView.right
        anchors.bottom: chatView.bottom
        height: Math.min(chatView.contentHeight + 10, parent.height)
        anchors.margins: -5
        opacity: 0.5
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
            Text {
                id: nameLabel
                font.bold: true
                font.pixelSize: 12
                text: model.name
            }
            Text {
                id: messageLabel
                font.pixelSize: 12
                text: model.message
                wrapMode: Text.Wrap
                anchors.left: nameLabel.right
                anchors.right: parent.right
                anchors.leftMargin: 10
            }
        }
    }
}
