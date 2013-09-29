import QtQuick 2.0
import Mana 1.0

Item {
    height: container.height;
    width: container.width;
    property string entryTitle: name == "" ? hostname : name;
    property bool canSee: false;

    Row {
        id: container;
        x: 5;
        y: 5;
        spacing: 5;

        Rectangle {
            id: status;
            width: 32;
            height: 32;
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
            client.connect(hostname, port);
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
        id: networkTimer;
        interval: 100;
        running: client.state !== AccountClient.Disconnected && client.state !== AccountClient.HostLookup;
        repeat: true;
        onTriggered: client.service();
    }

    Timer {
        id: disconnectTimer
        interval: 1
        onTriggered: client.disconnect();
    }

    Component.onCompleted: client.connect(hostname, port);
}
