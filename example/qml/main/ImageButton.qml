import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    id: button;

    property alias imagePath: image.source

    width: 32;
    height: 32;

    style: ButtonStyle {
        background: Image {
            smooth: false;
            source: {
                var baseName = "images/roundbutton";
                if (control.pressed)
                    return baseName + "_pressed.png";
                return baseName + ".png";
            }
        }
    }

    Image {
        id: image
        smooth: false;
        anchors.centerIn: parent;
        source: imagePath;
    }

    states: [
        State {
            name: "pressed"
            when: button.pressed
            PropertyChanges {
                target: button;
                anchors.horizontalCenterOffset: 1;
                anchors.verticalCenterOffset: 1;
            }
        }
    ]
}
