import Qt 4.7
import Tiled 1.0

Rectangle {
    width: 640;
    height: 480;

    color: "black";

    Flickable {
        id: flickable;

        anchors.fill: parent

        contentX: player.x - width / 2;
        contentY: player.y - height / 2;
        contentWidth: map.width;
        contentHeight: map.height;

        TileMap {
            id: map;
            source: gameClient.currentMap;

            opacity: status == TileMap.Ready ? 1 : 0
            Behavior on opacity { NumberAnimation {} }
        }

        Rectangle {
            id: player;
            color: "red";
            width: 32;
            height: 64;
        }
    }

    Text {
        color: "white";
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.margins: 5;
        text: "Current map: " + gameClient.currentMap;
    }

    Connections {
        target: gameClient;
        onMapChanged: {
            print(x, y)
            player.x = x;
            player.y = y;
        }
    }
}
