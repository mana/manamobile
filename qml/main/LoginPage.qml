import Qt 4.7
import Mana 1.0

Item {
    ErrorLabel {
        id: errorLabel;
        anchors.bottom: column.top
        anchors.bottomMargin: 20
        anchors.left: column.left
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

        enabled: accountClient.state == AccountClient.Connected && !loggingIn && !loggedIn

        onClicked: {
            print("Logging in as " + nameEdit.text + "...")
            loggingIn = true
            errorLabel.clear()
            accountClient.login(nameEdit.text, passwordEdit.text)
        }
    }

    Connections {
        target: accountClient;
        onLoginFailed: {
            errorLabel.showError(errorMessage);
        }
    }
}
