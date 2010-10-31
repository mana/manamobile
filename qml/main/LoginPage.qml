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
        anchors.verticalCenterOffset: 10;
        x: parent.width * 0.5
        spacing: 10

        LineEdit {
            id: nameEdit
            width: window.width * 0.4
            focus: true
            labelText: qsTr("Username");

            KeyNavigation.down: passwordEdit;
            backtabTarget: loginButton;
        }

        LineEdit {
            id: passwordEdit
            width: nameEdit.width
            labelText: qsTr("Password");
            echoMode: TextInput.Password

            KeyNavigation.up: nameEdit;
            KeyNavigation.down: loginButton;
        }

        Item { height: 1; width: 1; } // spacer

        Button {
            id: loginButton;
            text: qsTr("Login");
            anchors.right: column.right;

            KeyNavigation.up: passwordEdit;
            tabTarget: nameEdit;

            enabled: accountClient.state == AccountClient.Connected &&
                     !loggingIn && !loggedIn;

            onClicked: login();
        }
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
