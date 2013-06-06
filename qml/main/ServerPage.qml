import QtQuick 2.0
import Mana 1.0

Item {
    id: serverPage;
    property bool serverChosen: false;

    Text {
        id: title;
        x: window.width / 3;
        y: 5;
        width: window.width * 2 / 3 - 5;
        text: qsTr("Choose a server...");
        font.pixelSize: 35;
    }

    Item {
        anchors {
            left: title.left;
            right: parent.right;
            top: title.bottom;
            bottom: parent.bottom;
            margins: 5;
        }

        BorderImage {
            anchors.fill: parent

            source: "images/deepbox.png"
            border.bottom: 5;
            border.top: 5;
            border.right: 5;
            border.left: 5;
        }

        Flickable {
            anchors { fill: parent; margins: 3; }
            clip: true;
            contentWidth: width;
            contentHeight: flickColumn.height;
            pressDelay: 1000;

            Column {
                id: flickColumn;
                width: parent.width;
                spacing: 5;

                Repeater {
                    model: serverListModel;
                    delegate: ServerListEntry {}
                }
            }
        }

        Item {
            anchors.centerIn: parent;
            visible: serverListModel.notfound;

            Text {
                id: errorLabel;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.bottom;
                text: qsTr("Error downloading server list");
                font.bold: true;
            }
        }
    }

    ServerListModel {
        id: serverListModel;
    }
}
