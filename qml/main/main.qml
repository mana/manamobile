import Qt 4.7

Rectangle {
    id: window
    color: "#fffcf2"
    width: 800
    height: 480

    Image {
        x: -width * 0.1
        y: parent.height - height * 0.9
        sourceSize: Qt.size(parent.width / 2,
                            parent.width / 2)
        source: "images/mana.svg"
        opacity: 0.5
    }

    Column {
        id: column
        anchors.verticalCenter: parent.verticalCenter
        x: parent.width * 0.5
        spacing: 10

        Text {
            text: qsTr("Username:")
        }
        LineEdit {
            id: nameEdit
            width: window.width * 0.3
            focus: true
        }

        Text {
            text: qsTr("Password:")
        }
        LineEdit {
            id: passwordEdit
            width: nameEdit.width
            echoMode: TextInput.Password
        }
    }

    Button {
        text: "Login"
        anchors.top: column.bottom
        anchors.topMargin: 20
        anchors.right: column.right
        onClicked: {
            print("Login clicked!")
        }
    }
}
