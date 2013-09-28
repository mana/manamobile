import QtQuick 2.0
import Mana 1.0

Rectangle {
    id: joystick

    width: 100
    height: 100

    radius: 10
    opacity: 0.5
    border.color: "black"
    border.width: 1

    readonly property real centerX: width / 2
    readonly property real centerY: height / 2

    property real targetX: centerX
    property real targetY: centerY

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: Qt.platform.os !== "android"
        onReleased: updateWalkDirection()
    }

    MultiPointTouchArea {
        id: touchArea
        anchors.fill: parent
        enabled: Qt.platform.os === "android"
        onReleased: updateWalkDirection()
        maximumTouchPoints: 1
        touchPoints: [ TouchPoint { id: touchPoint } ]
    }

    Rectangle {
        width: 30
        height: 30
        radius: 5
        anchors.centerIn: parent
        color: "black"
        opacity: 0.3
    }

    Item {
        id: dot

        x: Math.max(0, Math.min(parent.width, targetX))
        y: Math.max(0, Math.min(parent.height, targetY))

        Rectangle {
            width: 10
            height: 10
            radius: 5
            anchors.centerIn: parent;
            border.color: "black";
            border.width: 1
        }
    }

    Binding {
        when: joystick.state != ""
        target: gameClient
        property: "playerWalkDirection"
        value: {
            var dx = targetX - centerX;
            var dy = targetY - centerY;

            Qt.point(Math.abs(dx) > 15 ? dx : 0,
                     Math.abs(dy) > 15 ? dy : 0);
        }
    }

    states: [
        State {
            name: "mouseAreaPressed"
            when: mouseArea.pressed
            PropertyChanges {
                target: joystick
                targetX: mouseArea.mouseX
                targetY: mouseArea.mouseY
            }
        },
        State {
            name: "touchPointPressed"
            when: touchPoint.pressed
            PropertyChanges {
                target: joystick
                targetX: touchPoint.x
                targetY: touchPoint.y
            }
        }
    ]
}
