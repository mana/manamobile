import QtQuick 2.0
import Tiled 1.0
import Mana 1.0

Rectangle {
    id: gamePage;
    width: 640;
    height: 480;

    property int centerX: width / 2;
    property int centerY: height / 2;

    // Workaround for QTBUG-28288 / QTBUG-25644, can be removed once Qt 4.8.5
    // or Qt 5.1.0 have been released.
    property bool chatBarHasActiveFocus: false;

    color: "black";

    Component.onCompleted: gamePage.forceActiveFocus();

    Item {
        id: mapContainer;

        property real smoothX;
        property real smoothY;

        // Remove fraction to avoid tile drawing glitches
        x: Math.floor(smoothX);
        y: Math.floor(smoothY);

        // This fast running timer may not be the best solution. One problem
        // with it is that it is not framerate agnostic.
        Timer {
            id: cameraTimer;
            interval: 10;
            running: gameClient.player !== null;
            repeat: true;
            onTriggered: {
                var containerX = gamePage.centerX - gameClient.player.x;
                var containerY = gamePage.centerY - gameClient.player.y;
                mapContainer.smoothX += (containerX - mapContainer.smoothX) * 0.1;
                mapContainer.smoothY += (containerY - mapContainer.smoothY) * 0.1;

                chatBarHasActiveFocus = chatBar.activeFocus;
            }
        }

        Connections {
            target: gameClient;
            onMapChanged: {
                // Immediately center the camera on the new player position
                mapContainer.smoothX = gamePage.centerX - x;
                mapContainer.smoothY = gamePage.centerY - y;
            }
        }

        TileMap {
            id: map;
            source: gameClient.currentMap;

            onStatusChanged: {
                if (status == TileMap.Ready) {
                    fadeInMap.start();
                } else {
                    fadeInMap.stop();
                    blackOverlay.opacity = 1;
                }
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

    Keys.onReturnPressed: chatBar.open();
    Keys.onEnterPressed: chatBar.open();

    onFocusChanged: {
        gameClient.playerWalkDirection = Qt.point(0, 0);
    }

    property bool w_pressed: false;
    property bool a_pressed: false;
    property bool s_pressed: false;
    property bool d_pressed: false;

    function updateWalkDirection() {
        var x = 0;
        var y = 0;

        if (w_pressed) --y;
        if (a_pressed) --x;
        if (s_pressed) ++y;
        if (d_pressed) ++x;

        gameClient.playerWalkDirection = Qt.point(x, y);
    }

    function handleKeyEvent(event, pressed) {
        if (event.isAutoRepeat)
            return;

        switch (event.key) {
        case Qt.Key_W: w_pressed = pressed; break;
        case Qt.Key_A: a_pressed = pressed; break;
        case Qt.Key_S: s_pressed = pressed; break;
        case Qt.Key_D: d_pressed = pressed; break;
        }

        updateWalkDirection();
    }

    Keys.onReleased: handleKeyEvent(event, false);
    Keys.onPressed: handleKeyEvent(event, true);

    NpcDialog {
        id: npcDialog;
        width: parent.width / 2;
        height: 100;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom;
    }

    FocusScope {
        id: chatBar;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        Keys.onReturnPressed: sayText();
        Keys.onEnterPressed: sayText();

        function open() {
            chatInput.focus = true;
            chatBar.focus = true;
        }

        function sayText() {
            if (chatInput.text != "") {
                gameClient.say(chatInput.text);
                chatInput.text = "";
            }
            gamePage.focus = true;
        }

        LineEdit {
            id: chatInput;
            anchors.left: parent.left;
            anchors.right: sayButton.left;
            tabTarget: sayButton;

            states: [
                State {
                    name: "opened";
                    when: chatBarHasActiveFocus;
                    PropertyChanges {
                        target: chatInput;
                        y: -chatInput.height - 5;
                    }
                }
            ]
            transitions: [
                Transition {
                    NumberAnimation {
                        property: "y";
                        easing.type: Easing.InOutQuad;
                    }
                }
            ]
        }
        Button {
            id: sayButton;
            anchors.right: parent.right;
            anchors.verticalCenter: chatInput.verticalCenter;
            text: "Say";
            onClicked: chatBar.sayText();
            KeyNavigation.left: chatInput;
        }
    }
}
