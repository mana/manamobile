import QtQuick 2.0
import Mana 1.0

Rectangle {
    width: 200;
    height: 200;
    radius: 20;
    opacity: 0.5;
    border.color: "black";
    border.width: 2;

    MouseArea {
        id: hitArea;
        anchors.fill: parent;
        onReleased: updateWalkDirection();

        Item {
            id: dot;
            state: "idle";

            Rectangle {
                width: 20;
                height: 20;
                radius: 10;
                anchors.centerIn: parent;
                border.color: "black";
                border.width: 2;
            }

            states: [
                State {
                    name: "idle";
                    AnchorChanges {
                        target: dot;
                        anchors.horizontalCenter: hitArea.horizontalCenter;
                        anchors.verticalCenter: hitArea.verticalCenter;
                    }
                },
                State {
                    name: "pressed";
                    when: hitArea.pressed;
                    PropertyChanges {
                        target: dot;
                        x: Math.max(0, Math.min(hitArea.width, hitArea.mouseX));
                        y: Math.max(0, Math.min(hitArea.height, hitArea.mouseY));
                    }
                }
            ]
        }

        Binding {
            when: hitArea.pressed;
            target: gameClient;
            property: "playerWalkDirection";
            value: Qt.point(hitArea.mouseX - hitArea.width / 2,
                            hitArea.mouseY - hitArea.height / 2);
        }
    }
}
