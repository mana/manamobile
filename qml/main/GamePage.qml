import QtQuick 2.0
import Mana 1.0

Item {
    id: gamePage;

    state: "game";

    // Workaround for QTBUG-28288 / QTBUG-25644, can be removed once Qt 4.8.5
    // or Qt 5.1.0 have been released.
    property bool chatBarHasActiveFocus: false;
    Timer {
        id: focusTimer;
        interval: 100;
        repeat: true;
        running: true;
        onTriggered: chatBarHasActiveFocus = chatBar.activeFocus;
    }

    Component.onCompleted: gamePage.forceActiveFocus();

    Viewport {
        id: viewport;
        width: parent.width / scale;
        height: parent.height / scale;
        scale: Math.ceil(Math.max(parent.width / 1200, parent.height / 1200));
        transformOrigin: Item.TopLeft;
    }

    MouseArea {
        id: mouse;
        anchors.fill: viewport;
        hoverEnabled: true;
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

        if (pressed && event.key == Qt.Key_C)
            gamePage.state = "status";

        if (pressed && event.key == Qt.Key_1)
            gameClient.useAbility(1, Qt.point(mouse.mouseX,
                                              mouse.mouseY));

        if (pressed && event.key == Qt.Key_2)
            gameClient.useAbility(3, Qt.point(mouse.mouseX,
                                              mouse.mouseY));
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

    StatusPage {
        id: statusPage;
        anchors.fill: parent;
        visible: false;
    }

    states: [
        State {
            name: "game";
            PropertyChanges {
                target: gamePage;
                focus: true;
            }
            PropertyChanges {
                target: statusPage;
                focus: false;
                opacity: 0;
            }
        },
        State {
            name: "status";
            PropertyChanges {
                target: statusPage;
                focus: true;
                visible: true;
                opacity: 1;
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
