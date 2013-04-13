import QtQuick 1.1
import Tiled 1.0
import Mana 1.0

Rectangle {
    id: gamePage;
    width: 640;
    height: 480;

    color: "black";
    focus: window.state == "game";

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

        // Smoothly animate the camera
        Behavior on contentX { SpringAnimation { spring: 3; damping: 1 } }
        Behavior on contentY { SpringAnimation { spring: 3; damping: 1 } }

        TileMap {
            id: map;
            source: gameClient.currentMap;

            opacity: status == TileMap.Ready ? 1 : 0
            Behavior on opacity { NumberAnimation {} }
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
                    anchors.bottom: sprite.top;
                    anchors.bottomMargin: 10;
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
                        if (model.being.type == Being.OBJECT_NPC) {
                            gameClient.npcDialogManager.startTalkingTo(model.being);
                        }
                    }
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

    Keys.onReturnPressed: chatBar.open();
    Keys.onEnterPressed: chatBar.open();

    onFocusChanged: {
        gameClient.playerWalkDirection = Qt.point(0, 0);
    }

    Keys.onReleased: {
        if (event.isAutoRepeat)
            return;

        var walkDirection = gameClient.playerWalkDirection;
        var x = walkDirection.x;
        var y = walkDirection.y;

        switch (event.key) {
        case Qt.Key_W: ++y; break;
        case Qt.Key_A: ++x; break;
        case Qt.Key_S: --y; break;
        case Qt.Key_D: --x; break;
        }
        gameClient.playerWalkDirection = Qt.point(x, y);
    }

    Keys.onPressed: {
        if (event.isAutoRepeat)
            return;

        var walkDirection = gameClient.playerWalkDirection;
        var x = walkDirection.x;
        var y = walkDirection.y;

        switch (event.key) {
        case Qt.Key_W: --y; break;
        case Qt.Key_A: --x; break;
        case Qt.Key_S: ++y; break;
        case Qt.Key_D: ++x; break;
        }
        gameClient.playerWalkDirection = Qt.point(x, y);
    }

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
                    when: chatBar.activeFocus;
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
