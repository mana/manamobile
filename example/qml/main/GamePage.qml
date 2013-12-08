import QtQuick 2.0
import Mana 1.0

Item {
    id: gamePage;

    state: "game";

    Component.onCompleted: gamePage.forceActiveFocus();

    PlayerAttributes { id: playerAttributes }

    Viewport {
        id: viewport;
        width: parent.width / scale;
        height: parent.height / scale;
        scale: window.gameScale
        transformOrigin: Item.TopLeft;
        centerX: visibleArea.x + visibleArea.width / 2;

        Item {
            id: visibleArea
            anchors.top: parent.top
            anchors.left: statusPage.right
            anchors.right: rightPanel.left
            anchors.bottom: chatLog.top
        }

        HealthBar {
            anchors.horizontalCenter: visibleArea.horizontalCenter;
            width: Math.min(200, visibleArea.width)
        }

        MouseArea {
            anchors.fill: parent;
            enabled: actionBar.selectedAbility();

            onClicked: {
                var ability = actionBar.selectedAbility();
                var mapPos = viewport.toMapPos(mouse.x, mouse.y);
                gameClient.useAbilityOnPoint(ability, mapPos.x, mapPos.y);

                actionBar.reset();
            }
        }

        RespawnButton {
            id: respawnButton
            anchors.centerIn: parent
            anchors.verticalCenterOffset: height / 2 + 16
        }

        ChatLog {
            id: chatLog
            anchors.left: joystick.right
            anchors.right: actionBar.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
        }

        NpcDialog {
            id: npcDialog;

            property bool narrow: viewport.width - 240 < 300

            width: Math.max(300, narrow ? viewport.width * 0.8 : (viewport.width - 240) * 0.9)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: narrow ? 120 : 50
        }
        ShopWindow {
            anchors.centerIn: visibleArea
        }

        StatusPage {
            id: statusPage
            anchors.top: parent.top
            anchors.bottom: chatLog.top
            anchors.topMargin: 3
            width: viewport.width / 2 - 22 - 32
        }

        RightPanel {
            id: rightPanel
            anchors.top: parent.top
            anchors.bottom: chatLog.top
            anchors.topMargin: 3
            width: viewport.width / 2 - 22 - 32
        }

        Joystick {
            id: joystick
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }

        ImageButton {
            imagePath: "images/chat_icon.png";
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 16;
            anchors.right: actionBar.left;
            anchors.rightMargin: 64;
            onClicked: chatBar.open();
        }

        ActionBar {
            id: actionBar;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
        }
    }

    Keys.onReturnPressed: {
        if (event.modifiers === Qt.NoModifier)
            chatBar.open();
        else
            event.accepted = false;
    }
    Keys.onEnterPressed: chatBar.open();

    onFocusChanged: {
        gameClient.playerWalkDirection = Qt.vector2d(0, 0);
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

        gameClient.playerWalkDirection = Qt.vector2d(x, y);
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

        if (pressed && event.key === Qt.Key_C)
            statusPage.toggle();

        if (pressed && event.key === Qt.Key_I)
            rightPanel.toggle("inventory");

        if (pressed && event.key === Qt.Key_Q)
            rightPanel.toggle("quest");
    }

    Keys.onReleased: handleKeyEvent(event, false);
    Keys.onPressed: handleKeyEvent(event, true);

    ChatBar {
        id: chatBar;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: Qt.inputMethod.visible ? 316 : 0
    }

    states: [
        State {
            name: "game";
            PropertyChanges {
                target: gamePage;
                focus: true;
            }
        }
    ]

    transitions: [
        Transition {
            to: "game";
            SequentialAnimation {
                NumberAnimation {
                    property: "opacity";
                    easing.type: Easing.InOutQuad;
                }
                PropertyAction { property: "visible" }
            }
        },
        Transition {
            NumberAnimation {
                property: "opacity";
                easing.type: Easing.InOutQuad;
            }
        }
    ]
}
