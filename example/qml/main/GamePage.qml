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
        scale: Math.ceil(Math.max(gamePage.width / 959, gamePage.height / 959));
        transformOrigin: Item.TopLeft;
        centerX: (statusPage.x + statusPage.width + inventoryPage.x) / 2;

        Item {
            id: visibleArea
            anchors.top: parent.top
            anchors.left: statusPage.right
            anchors.right: inventoryPage.left
            anchors.bottom: parent.bottom
        }

        HealthBar {
            y: 5;
            anchors.horizontalCenter: visibleArea.horizontalCenter;
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
            maxHeight: window.height - respawnButton.y - respawnButton.height;
            anchors.left: joystick.right
            anchors.right: actionBar.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.bottomMargin: 10
        }

        NpcDialog {
            id: npcDialog;
            width: Math.max(parent.width / 2, Math.min(300, parent.width - 20));
            height: 100;
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 5;
        }

        StatusPage { id: statusPage; }
        InventoryPage { id: inventoryPage; }

        Joystick {
            id: joystick
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 20
            anchors.bottomMargin: 20
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
            anchors.bottomMargin: 16;
            anchors.right: parent.right;
            anchors.rightMargin: 16;
        }
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

        if (pressed && event.key === Qt.Key_C)
            statusPage.toggle();

        if (pressed && event.key === Qt.Key_I)
            inventoryPage.toggle();

        if (pressed && event.key === Qt.Key_Q)
            questPage.toggle();
    }

    Keys.onReleased: handleKeyEvent(event, false);
    Keys.onPressed: handleKeyEvent(event, true);

    QuestPage { id: questPage; }

    FocusScope {
        id: chatBar;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        Keys.onReturnPressed: sayText();
        Keys.onEnterPressed: sayText();

        function open() {
            chatBar.focus = true;
        }

        function sayText() {
            if (chatInput.text != "") {
                gameClient.say(chatInput.text);
                chatInput.text = "";
            }

            chatInput.focus = false;
            Qt.inputMethod.hide();
            gamePage.focus = true;
        }

        LineEdit {
            id: chatInput;
            anchors.left: parent.left;
            anchors.right: sayButton.left;

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
                    SequentialAnimation {
                        NumberAnimation {
                            property: "y";
                            easing.type: Easing.InOutQuad;
                        }
                        ScriptAction {
                            script: chatInput.focus = chatInput.y !== 0;
                        }
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
