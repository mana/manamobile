import QtQuick 1.1
import Mana 1.0

Item {
    id: serverPage;
    property bool serverChosen: false;
    anchors.fill: parent;

    Text {
        id: title;
        x: window.width / 3;
        y: 5;
        width: window.width * 2 / 3 - 5;
        text: qsTr("Choose a server...");
        font.pixelSize: 35;
    }

    Item {
        anchors { left: title.left; right: parent.right; top: title.bottom;
                    bottom: parent.bottom; margins: 5; }
        height: window.height / 2;

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

                Repeater {
                    //anchors.fill: parent;
                    model: serverListModel;
                    delegate: serverListEntry;
                }
            }
        }
    }

    Timer {
        id: networkTimer;
        interval: 100;
        running: true;
        repeat: true;
    }

    Component {
        id: serverListEntry
        Item {
            id: serverListEntryItem;
            height: container.height;
            width: flickColumn.width - 4;
            property string entryTitle: name == "" ? hostname : name;
            property bool canSee: false;

            Row {
                id: container;
                x: 1;
                spacing: 2;

                Rectangle {
                    id: status;
                    width: 32;
                    height: 32;
                    y: nameLabel.y + 5;

                    color: canSee ? "green" : "red";
                }

                Column {
                    spacing: 1;
                    Text {
                        id: nameLabel;
                        text: entryTitle;
                        font.pointSize: 12;
                    }

                    Text {
                        id: addressLabel;
                        text: hostname + ":" + port;
                        color: "midnightblue";
                        font.pointSize: 9;
                        z: 5
                    }
                }
            }

            MouseArea {
                anchors.fill: parent;
                enabled: !serverPage.serverChosen;
                onClicked: {
                    serverPage.serverChosen = true;
                    serverName = entryTitle;
                    accountClient.connect(hostname, port);
                }
            }

            AccountClient {
                id: client;
                onConnected: {
                    canSee = true;

                    // A direct disconnect here messes things up
                    disconnectTimer.start();
                }
            }

            Timer {
                id: disconnectTimer
                interval: 1
                onTriggered: client.disconnect();
            }

            Component.onCompleted: {
                client.connect(hostname, port);
            }

            Connections {
                target: networkTimer;
                onTriggered: {
                    client.service();
                }
            }
        }
    }

    XmlListModel {
        id: serverListModel;
        source: "http://manasource.org/serverlist.xml";
        query: "/serverlist/server[lower-case(@type)='manaserv']";

        XmlRole { name: "name"; query: "@name/string()"; }
        XmlRole { name: "hostname"; query: "connection/@hostname/string()"; }
        XmlRole { name: "port"; query: "connection/@port/number()"; }
        XmlRole { name: "description"; query: "description/string()"; }
    }
}
