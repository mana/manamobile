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

            KeyNavigation.down: passwordEdit;
            backtabTarget: loginButton;
        }

        Text {
            text: qsTr("Password:")
        }
        LineEdit {
            id: passwordEdit
            width: nameEdit.width
            echoMode: TextInput.Password

            KeyNavigation.up: nameEdit;
            KeyNavigation.down: loginButton;
        }
    }

    Button {
        id: loginButton
        text: "Login"
        anchors.top: column.bottom
        anchors.topMargin: 20
        anchors.right: column.right

        KeyNavigation.up: passwordEdit;
        tabTarget: nameEdit;

        enabled: accountClient.state == AccountClient.Connected && !loggingIn && !loggedIn

        onClicked: login();
    }

    Keys.onReturnPressed: login();

    function login() {
        print("Logging in as " + nameEdit.text + "...")
        loggingIn = true
        errorLabel.clear()
        accountClient.login(nameEdit.text, passwordEdit.text)
    }

    Connections {
        target: accountClient;
        onLoginFailed: {
            errorLabel.showError(errorMessage);
        }
    }
}
