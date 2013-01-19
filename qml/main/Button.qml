import QtQuick 1.1

/**
 * A Button in Mana style
 */
BorderImage {
    id: button

    property alias text: label.text
    property alias enabled: mouseArea.enabled
    property variant tabTarget: KeyNavigation.right || KeyNavigation.down;
    property variant backtabTarget: KeyNavigation.left || KeyNavigation.up;
    property string baseName: "images/bigbutton";

    signal clicked

    width: Math.max(label.width + 20 + 20, 200);
    anchors.margins: 5;

    source: baseName + ".png";
    border.bottom: 20;
    border.top: 26;
    border.right: 100;
    border.left: 100;

    QtObject {
        id: priv;
        property bool spaceDown: false;
    }

    Keys.onReleased: {
        if (event.key == Qt.Key_Space && !event.isAutoRepeat) {
            button.clicked();
            priv.spaceDown = false;
            event.accepted = true;
        }
    }
    Keys.onSpacePressed: priv.spaceDown = true;
    Keys.onTabPressed: if (tabTarget) tabTarget.focus = true;
    Keys.onBacktabPressed: if (backtabTarget) backtabTarget.focus = true;

    TextShadow {
        target: label;
        color: "white";
        opacity: 0.7;
    }
    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: 35;
        opacity: 0.8;
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        anchors.bottomMargin: -5;
        hoverEnabled: true
        onClicked: button.clicked()
    }

    states: [
        State {
            name: "pressed"
            when: enabled && ((mouseArea.containsMouse && mouseArea.pressed) ||
                              priv.spaceDown);
            PropertyChanges {
                target: button
                source: baseName + "_pressed.png";
            }
            PropertyChanges {
                target: label
                anchors.horizontalCenterOffset: 1
                anchors.verticalCenterOffset: 1
            }
        },
        State {
            name: "hovered"
            when: enabled && ((mouseArea.containsMouse && !mouseArea.pressed) ||
                              button.activeFocus);
            PropertyChanges {
                target: button
                source: baseName + "_hovered.png";
            }
        },
        State {
            name: "disabled"
            when: !enabled
            PropertyChanges {
                target: button
                source: baseName + "_disabled.png";
            }
            PropertyChanges {
                target: label;
                opacity: 0.7;
            }
        }
    ]
}
