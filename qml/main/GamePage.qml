import Qt 4.7
import Tiled 1.0

Rectangle {
    width: 640;
    height: 480;

    color: "black";

    MouseArea {
        id: walkMouseArea;
        anchors.fill: parent;

        function walkToMouse() {
            gameClient.walkTo(walkMouseArea.mouseX + mapView.contentX,
                              walkMouseArea.mouseY + mapView.contentY);
        }

        onClicked: walkToMouse();

        Timer {
            interval: 250;
            running: walkMouseArea.pressed;
            triggeredOnStart: true;
            repeat: true;
            onTriggered: walkMouseArea.walkToMouse();
        }
    }

    Flickable {
        id: mapView;

        anchors.fill: parent
        interactive: false;

        // Center the view on the player
        contentX: {
            if (!gameClient.player)
                return 0;

            var centeredPos = gameClient.player.x - width / 2;
            return Math.max(0, Math.min(contentWidth - width, centeredPos));
        }
        contentY: {
            if (!gameClient.player)
                return 0;

            var centeredPos = gameClient.player.y - height / 2;
            return Math.max(0, Math.min(contentHeight - height, centeredPos));
        }
        contentWidth: map.width;
        contentHeight: map.height;

        TileMap {
            id: map;
            source: gameClient.currentMap;

            opacity: status == TileMap.Ready ? 1 : 0
            Behavior on opacity { NumberAnimation {} }
        }

        Repeater {
            model: gameClient.beingListModel;
            delegate: Item {
                x: model.x;
                y: model.y;
                z: y;

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.bottom: parent.bottom;
                    color: "blue";
                    width: 32;
                    height: 64;
                    opacity: 0.5;
                    radius: 15;
                }

                Text {
                    anchors.top: parent.bottom
                    anchors.topMargin: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: model.name;
                }
            }
        }
    }

    Rectangle {
        color: "black";
        opacity: 0.5;
        anchors.fill: mapName;
        anchors.margins: -4;
        radius: 4;
    }
    Text {
        id: mapName;
        color: "white";
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.margins: 2;
        text: "Current map: " + gameClient.currentMap;
    }
}
