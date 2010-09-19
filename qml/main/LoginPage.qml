import Qt 4.7

Item {
    Text {
        id: errorLabel
        anchors.bottom: column.top
        anchors.bottomMargin: 20
        anchors.left: column.left

        text: errorMessage
        color: "red"
        font.bold: true

        onTextChanged: {
            scaleAnimation.running = true
        }

        ParallelAnimation {
            id: scaleAnimation
            NumberAnimation {
                target: errorLabel;
                property: "scale"; from: 2; to: 1;
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                target: errorLabel;
                property: "opacity"; from: 0; to: 1;
                easing.type: Easing.OutQuad
            }
        }
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
        id: loginButton
        text: "Login"
        anchors.top: column.bottom
        anchors.topMargin: 20
        anchors.right: column.right

        enabled: loginManager.connected && !loggingIn && !loggedIn

        onClicked: {
            print("Logging in as " + nameEdit.text + "...")
            loggingIn = true
            errorMessage = ""
            loginManager.login(nameEdit.text, passwordEdit.text)
        }
    }
}
