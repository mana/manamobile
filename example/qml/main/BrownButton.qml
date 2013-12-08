import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

/**
 * A brown button suitable for use on scrolls.
 */
Button {
    id: button

    implicitHeight: Math.max(sizeLabel.height + 10, 30)

    property bool keepPressed: false

    style: ButtonStyle {
        background: BorderImage {
            source: {
                if (control.pressed || button.keepPressed)
                    "images/scroll_button_pressed.png"
                else
                    "images/scroll_button.png"
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
                color: "#3f2b25"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.rightMargin: 7
                anchors.leftMargin: 7
                font.pixelSize: 14
                wrapMode: Text.WordWrap
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
                    when: control.pressed || button.keepPressed
                    PropertyChanges {
                        target: label
                        anchors.verticalCenterOffset: 1
                    }
                    PropertyChanges {
                        target: icon
                        anchors.verticalCenterOffset: 1
                    }
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
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.rightMargin: 7
        anchors.leftMargin: 7
        visible: false
        text: button.text
        font.pixelSize: 14
        wrapMode: Text.WordWrap
    }
}
