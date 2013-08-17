import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

/**
 * A Button in Mana style
 */
Button {
    id: button

    property string baseName: "images/bigbutton";

    implicitWidth: Math.max(sizeLabel.width + 20 + 20, 200);

    style: ButtonStyle {
        background: BorderImage {

            source: {
                if (control.pressed)
                    baseName + "_pressed.png";
                else if (!control.enabled)
                    baseName + "_disabled.png";
                else if (control.hovered || control.activeFocus)
                    baseName + "_hovered.png";
                else
                    baseName + ".png";
            }

            border.bottom: 20;
            border.top: 26;
            border.right: 100;
            border.left: 100;
        }

        label: Item {
            TextShadow {
                target: label;
                color: "white";
                opacity: 0.7;
            }
            Text {
                id: label
                text: control.text
                anchors.centerIn: parent
                font.pixelSize: 35
                opacity: 0.8;
            }

            states: [
                State {
                    name: "pressed"
                    when: control.pressed
                    PropertyChanges {
                        target: label
                        anchors.horizontalCenterOffset: 1
                        anchors.verticalCenterOffset: 1
                    }
                },
                State {
                    name: "disabled"
                    when: !control.enabled
                    PropertyChanges {
                        target: label;
                        opacity: 0.7;
                    }
                }
            ]
        }
    }

    Text {
        id: sizeLabel
        visible: false
        text: button.text
        font.pixelSize: 35
    }
}
