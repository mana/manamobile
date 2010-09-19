import Qt 4.7

/**
 * A Button in Mana style
 */
BorderImage {
    id: button

    property alias text: label.text
    property alias enabled: mouseArea.enabled

    signal clicked

    width: label.width + 7 + 7

    source: "images/button.png"
    border.bottom: 4
    border.top: 4
    border.right: 4
    border.left: 4

    Text {
        id: label
        anchors.centerIn: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: button.clicked()
    }

    states: [
        State {
            name: "pressed"
            when: enabled && mouseArea.containsMouse && mouseArea.pressed
            PropertyChanges {
                target: button
                source: "images/buttonpress.png"
            }
            PropertyChanges {
                target: label
                anchors.horizontalCenterOffset: 1
                anchors.verticalCenterOffset: 1
            }
        },
        State {
            name: "hovered"
            when: enabled && mouseArea.containsMouse && !mouseArea.pressed
            PropertyChanges {
                target: button
                source: "images/buttonhi.png"
            }
        },
        State {
            name: "disabled"
            when: !enabled
            PropertyChanges {
                target: button
                source: "images/button_disabled.png"
            }
        }
    ]
}
