import Qt 4.7
import Mana 1.0

Item {
    id: serverPage;
    property bool serverChosen: false;

    Column {
        id: column;
        x: window.width / 3;
        y: 0;
        width: window.width * 2;
        spacing: 10;

        Text {
            id: title;
            text: qsTr("Choose a server...");
            font.pixelSize: 35;
        }

        Item {
            anchors { left: parent.left; right: parent.right; }
            height: window.height / 2;

            BorderImage {
                anchors.fill: parent

                source: "images/deepbox.png"
                border.bottom: 5;
                border.top: 5;
                border.right: 5;
                border.left: 5;
            }

            ListView {
                id: serverList;
                focus: window.state == "chooseServer";
                anchors { fill: parent; margins: 3; }

                model: serverListModel;
                delegate: Item {
                    height: row.height + 2;
                    Row {
                        id: row;
                        x: 1;
                        spacing: 2;

                        Rectangle {
                            width: 20;
                            height: 20;

                            // TODO Query the server for status
                            color: "green";
                        }

                        Text {
                            text: name;
                        }
                    }

                    MouseArea {
                        anchors.fill: row
                        onClicked: {
                            if (!serverPage.serverChosen) {
                                serverPage.serverChosen = true;
                                serverList.currentIndex = model.index;
                                serverName = name == "" ? hostname : name;
                                connect(hostname, port);
                            }
                        }
                    }
                }
                highlight: Rectangle {
                    color: "black";
                    opacity: 0.2;
                    width: serverList.width;
                }

                onCountChanged: {
                    // Select first item when possible
                    if (currentIndex == -1 && count > 0)
                        currentIndex = 0;
                }

                XmlListModel {
                    id: serverListModel;
                    source: "http://manasource.org/serverlist.xml";
                    query: "/serverlist/server[lower-case(@type)='manaserv']";

                    XmlRole { name: "name"; query: "@name/string()" }
                    XmlRole { name: "hostname"; query: "connection/@hostname/string()" }
                    XmlRole { name: "port"; query: "connection/@port/number()" }
                    XmlRole { name: "description"; query: "description/string()" }
                }
            }
        }
    }
}
