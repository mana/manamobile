import QtQuick 2.0
import Mana 1.0

Item {
    id: gamePage;

    state: "game";

    Component.onCompleted: gamePage.forceActiveFocus();

    PlayerAttributes { id: playerAttributes }

    Item {
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        Viewport {
            id: viewport;
            width: parent.width / scale;
            height: parent.height / scale;
            scale: Math.ceil(Math.max(gamePage.width / 1200, gamePage.height / 1200));
            transformOrigin: Item.TopLeft;
            centerX: (statusPage.x + statusPage.width + inventoryPage.x) / 2;

            HealthBar {
                y: 5;
                anchors.horizontalCenter: parent.horizontalCenter;
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

            ChatLog {
                anchors.top: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
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
        text: "Current map: " + gameClient.currentMapName;
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

    Joystick {
        id: joystick;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        anchors.leftMargin: 50;
        anchors.bottomMargin: 50;
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

    ActionBar {
        id: actionBar;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
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
