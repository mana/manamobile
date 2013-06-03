import QtQuick 2.0
import Mana 1.0

/**
 * The main viewport, showing the map and entities.
 */
Item {
    id: viewport;

    property real centerX: width / 2;
    property real centerY: height / 2;

    function screenToMap(x, y) {
        return map.mapFromItem(null, x, y);
    }

    // This fast running timer may not be the best solution. One problem
    // with it is that it is not framerate agnostic.
    Timer {
        id: cameraTimer;
        interval: 10;
        running: gameClient.player !== null;
        repeat: true;
        onTriggered: {
            var containerX = viewport.centerX - gameClient.player.x;
            var containerY = viewport.centerY - gameClient.player.y;
            map.smoothX += (containerX - map.smoothX) * 0.1;
            map.smoothY += (containerY - map.smoothY) * 0.1;
        }
    }

    Connections {
        target: gameClient;
        onMapChanged: {
            // Immediately center the camera on the new player position
            map.smoothX = viewport.centerX - x;
            map.smoothY = viewport.centerY - y;
        }
    }

    TileMap {
        id: map;
        source: gameClient.currentMap;

        property real smoothX;
        property real smoothY;

        // Remove fraction to avoid tile drawing glitches
        x: Math.floor(smoothX);
        y: Math.floor(smoothY);

        visibleArea: Qt.rect(-map.x,
                             -map.y,
                             viewport.width,
                             viewport.height);

        onStatusChanged: {
            if (status == TileMap.Ready) {
                fadeInMap.start();
            } else {
                fadeInMap.stop();
                blackOverlay.opacity = 1;
            }
        }

        Repeater {
            model: gameClient.beingListModel;
            delegate: Item {
                x: model.being.x;
                y: model.being.y;
                z: y;

                Rectangle {
                    anchors.fill: chat;
                    anchors.margins: -4;
                    radius: 10;
                    color: Qt.rgba(0, 0, 0, 0.2);
                    opacity: chat.opacity;
                }
                TextShadow { target: chat; }
                Text {
                    id: chat;
                    anchors.bottom: sprite.bottom;
                    anchors.bottomMargin: sprite.maxHeight;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    text: model.being.chatMessage;
                    color: "white";
                    opacity: 0;
                    font.pixelSize: 15;

                    onTextChanged: {
                        opacity = 1;
                        chatAnimation.restart();
                    }

                    SequentialAnimation {
                        id: chatAnimation;
                        PauseAnimation { duration: Math.min(10000, 2500 + chat.text.length * 50); }
                        NumberAnimation { target: chat; property: "opacity"; to: 0; }
                    }
                }

                CompoundSprite {
                    id: sprite;
                    sprites: model.being.spriteListModel;
                    action: model.being.action;
                    direction: model.being.spriteDirection;
                }

                Text {
                    anchors.top: parent.bottom
                    anchors.topMargin: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: model.being.name;
                    font.pixelSize: 12;
                }

                MouseArea {
                    width: 64;
                    height: 64;

                    anchors.bottom: parent.bottom;
                    anchors.horizontalCenter: parent.horizontalCenter;

                    enabled: model.being.type === Being.OBJECT_NPC;
                    onClicked: {
                        gameClient.npcDialogManager.startTalkingTo(model.being);
                    }
                }
            }
        }
    }

    Rectangle {
        id: blackOverlay;
        color: "black";
        anchors.fill: parent;

        NumberAnimation on opacity { id: fadeInMap; to: 0; }
    }
}
