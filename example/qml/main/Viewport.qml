import QtQuick 2.0
import Mana 1.0

/**
 * The main viewport, showing the map and entities.
 */
Rectangle {
    id: viewport;

    // TODO: Take background color from the map when available. Maybe having
    // MapItem draw a background when necessary.
    color: "black"

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
        QtObject {
            id: smoothFollow;
            property real smoothPlayerX: viewport.playerX;
            property real smoothPlayerY: viewport.playerY;
            property real mapX: viewport.centerX - smoothPlayerX;
            property real mapY: viewport.centerY - smoothPlayerY;

            Behavior on smoothPlayerX { SpringAnimation { spring: 3; damping: 1 } }
            Behavior on smoothPlayerY { SpringAnimation { spring: 3; damping: 1 } }

            // Math.floor is used to avoid tile drawing glitches
            property QtObject bindingX: Binding {
                target: map; property: "x";
                value: {
                    var scale = viewport.scale;
                    Math.floor(smoothFollow.mapX * scale) / scale;
                }
            }
            property QtObject bindingY: Binding {
                target: map; property: "y";
                value: {
                    var scale = viewport.scale;
                    Math.floor(smoothFollow.mapY * scale) / scale;
                }
            }
        }
    }

    function toMapPos(viewportX, viewportY) {
        return viewport.mapToItem(map, viewportX, viewportY);
    }

    property QtObject smoothFollowInstance;

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
        mapResource: gameClient.currentMapResource;

        visible: status == TileMap.Ready
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
            model: gameClient.dropListModel;
            delegate: Item {
                x: model.drop.position.x;
                y: model.drop.position.y;
                z: y;

                property variant itemInfo: itemDB.isLoaded ?
                                               itemDB.getInfo(model.drop.id) :
                                               undefined;
                Image {
                    anchors.centerIn: parent;
                    source: itemInfo ?
                                resourceManager.dataUrl + resourceManager.itemIconsPrefix + itemInfo.image :
                                undefined;
                    smooth: false;
                }
            }
        }

        Repeater {
            model: gameClient.beingListModel;
            delegate: Item {
                x: model.being.x;
                y: model.being.y;
                z: y;

                CompoundSprite {
                    id: sprite;
                    being: model.being
                }

                MouseArea {
                    width: 64;
                    height: 64;

                    anchors.bottom: parent.bottom;
                    anchors.bottomMargin: -16
                    anchors.horizontalCenter: parent.horizontalCenter;

                    onClicked: {
                        if (model.being.type === Being.OBJECT_NPC)
                            gameClient.talkToNpc(model.being);
                    }
                }

                // Player name and chat messages are displayed above the map
                Item {
                    parent: map;
                    x: model.being.x;
                    y: model.being.y;
                    z: 65537; // Layers above the Fringe layer have z 65536

                    OverheadChatMessage {
                        id: chatLabel;
                        anchors.bottom: parent.bottom;
                        anchors.bottomMargin: sprite.maxHeight;
                    }

                    TextShadow {
                        target: nameLabel
                    }
                    Text {
                        id: nameLabel
                        anchors.top: parent.bottom
                        anchors.topMargin: 12
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: model.being.name;
                        textFormat: Text.PlainText
                        font.pixelSize: 12;
                        color: {
                            if (model.being.type === Being.OBJECT_NPC)
                                return "DeepSkyBlue";
                            else
                                return "white";
                        }
                        font.bold: true
                        visible: {
                            var type = model.being.type;
                            if (type === Being.OBJECT_NPC)
                                return true;
                            if (type === Being.OBJECT_CHARACTER)
                                return model.being !== gameClient.player;
                            return false;
                        }
                    }
                }

                Connections {
                    target: model.being;
                    onAbilityUsed: sprite.restartAction();
                    onChatMessage: chatLabel.showText(message);
                    onDamageTaken: {
                        console.log(being.name + " takes " + amount + " damage")
                        hitAnimation.restart()
                    }
                }

                SequentialAnimation {
                    id: hitAnimation
                    NumberAnimation {
                        target: sprite; property: "x"; duration: 50; to: 5
                        easing.type: Easing.InOutSine
                    }
                    SequentialAnimation {
                        loops: 4
                        NumberAnimation {
                            target: sprite; property: "x"; duration: 50; to: -5
                            easing.type: Easing.InOutSine
                        }
                        NumberAnimation {
                            target: sprite; property: "x"; duration: 50; to: 5
                            easing.type: Easing.InOutSine
                        }
                    }
                    NumberAnimation {
                        target: sprite; property: "x"; duration: 50; to: 0
                        easing.type: Easing.InOutSine
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
