import QtQuick 2.0
import Mana 1.0

Rectangle {
    width: 100
    height: 100
    radius: 10
    opacity: 0.5;
    border.color: "black";
    border.width: 1

    MouseArea {
        id: hitArea;
        anchors.fill: parent;
        onReleased: updateWalkDirection();

        Item {
            id: dot;
            state: "idle";

            Rectangle {
                width: 10
                height: 10
                radius: 5
                anchors.centerIn: parent;
                border.color: "black";
                border.width: 1
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
