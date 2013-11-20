import QtQuick 2.0

MouseArea {
    id: screen

    anchors.fill: parent

    property alias title: titleLabel.text
    property alias message: messageLabel.text

    property alias confirmText: confirmButton.text
    property alias cancelText: cancelButton.text

    signal confirmed
    signal canceled

    cursorShape: Qt.ArrowCursor

    Rectangle {
        id: background
        anchors.fill: parent
        color: "black"
        opacity: 0.5
    }

    Item {
        id: dialog

        anchors.centerIn: parent
        width: messageLabel.width + 30
        height: messageLabel.contentHeight + 46 + 50

        Rectangle {
            anchors.fill: parent
            anchors.margins: 3
            color: "black"
            opacity: 0.6
        }

        BorderImage {
            id: frame
            anchors.fill: parent
            source: "images/frame_with_title.png"
            border.left: 36; border.top: 30
            border.right: 36; border.bottom: 15
            smooth: false
        }

        Text {
            clip: false
            id: titleLabel
            font.pixelSize: 10
            font.bold: true
            color: "white"
            font.capitalization: Font.SmallCaps
            anchors.horizontalCenter: frame.horizontalCenter
            anchors.top: frame.top
            anchors.topMargin: 5
        }

        TextShadow { target: messageLabel }
        Text {
            id: messageLabel
            color: "#c8e8e8"
            font.pixelSize: 14
            anchors.top: frame.top
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: 8
            horizontalAlignment: Text.AlignHCenter
            width: Math.max(250, confirmButton.width + cancelButton.width + 20)
            wrapMode: Text.WordWrap
        }

        BlueButton {
            id: cancelButton
            text: qsTr("Cancel")
            anchors.left: frame.left
            anchors.bottom: frame.bottom
            anchors.margins: 20
            onClicked: {
                screen.visible = false;
                canceled();
            }
            visible: text != ""
        }

        BlueButton {
            id: confirmButton
            text: qsTr("OK")
            anchors.right: frame.right
            anchors.bottom: frame.bottom
            anchors.margins: 20
            onClicked: {
                screen.visible = false;
                confirmed();
            }
        }
    }

    states: [
        State {
            name: "invisible"
            when: !screen.visible
            PropertyChanges {
                target: screen
                opacity: 0
            }
            PropertyChanges {
                target: dialog
                scale: 0.75
            }
        },
        State {
            name: "visible"
            when: screen.visible
        }
    ]

    transitions: [
        Transition {
            to: "visible"
            NumberAnimation {
                properties: "opacity,scale"; easing.type: Easing.OutBack
            }
        }
    ]
}
