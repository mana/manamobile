import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

/**
 * A blue button suitable for use on frames.
 */
Button {
    id: button

    implicitHeight: Math.max(sizeLabel.height + 10, 30)
    implicitWidth: Math.max(sizeLabel.width + 20, 30)

    property bool keepPressed: false

    style: ButtonStyle {
        background: BorderImage {
            source: {
                if (control.pressed || button.keepPressed)
                    "images/frame_button_pressed.png"
                else
                    "images/frame_button.png"
            }

            smooth: false

            border.bottom: 5
            border.top: 5
            border.right: 5
            border.left: 5
        }

        label: Item {
            Text {
                id: label
                text: control.text
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: 14
            }

            Image {
                id: icon
                source: control.iconSource
                anchors.centerIn: parent
                smooth: false
            }

            states: [
                State {
                    name: "pressed"
                    when: control.pressed
                },
                State {
                    name: "disabled"
                    when: !control.enabled
                    PropertyChanges {
                        target: label
                        opacity: 0.7
                    }
                }
            ]
        }
    }

    Text {
        id: sizeLabel
        visible: false
        text: button.text
        font.pixelSize: 14
    }
}
