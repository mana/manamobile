import QtQuick 2.0
import Mana 1.0

/**
 * The main viewport, showing the map and entities.
 */
Item {
    id: viewport;

    property real centerX: width / 2;
    property real centerY: height / 2;
    property real playerX: gameClient.player ? gameClient.player.x : gameClient.playerStartX;
    property real playerY: gameClient.player ? gameClient.player.y : gameClient.playerStartY;

    // There seems to be no good way to temporarily disable a Behavior. So in
    // order to avoid the smooth following of the player on warps, this
    // this component is re-created, resetting the animation to start from the
    // new player location.
    Component {
        id: smoothFollowComponent;
        Item {
            id: smoothFollow;
            property real mapX: viewport.centerX - viewport.playerX;
            property real mapY: viewport.centerY - viewport.playerY;

            Behavior on mapX { SpringAnimation { spring: 3; damping: 1 } }
            Behavior on mapY { SpringAnimation { spring: 3; damping: 1 } }

            // Math.floor is used to avoid tile drawing glitches
            Binding { target: map; property: "x"; value: Math.floor(smoothFollow.mapX); }
            Binding { target: map; property: "y"; value: Math.floor(smoothFollow.mapY); }
        }
    }

    property var smoothFollowInstance;

    function resetSmoothFollow() {
        if (smoothFollowInstance)
            smoothFollowInstance.destroy();

        smoothFollowInstance = smoothFollowComponent.createObject(viewport);
    }

    Component.onCompleted: resetSmoothFollow();
    Connections {
        target: gameClient;
        onMapChanged: resetSmoothFollow();
    }

    TileMap {
        id: map;
        source: gameClient.currentMap;

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

                    onClicked: {
                        if (model.being.type === Being.OBJECT_NPC)
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
