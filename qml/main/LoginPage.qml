import Qt 4.7
import Mana 1.0

Item {
    id: loginPage;
    Settings { id: settings }

    function login() {
        print("Logging in as " + nameEdit.text + "...");
        loggingIn = true;
        errorLabel.clear();
        settings.setValue("username", nameEdit.text)
        accountClient.login(nameEdit.text, passwordEdit.text);
    }

    function register() {
        if (passwordEdit.text != passwordConfirmEdit.text) {
            errorLabel.showError(qsTr("Passwords do not match"))
            return;
        }

        loggingIn = true;
        errorLabel.clear();
        var captchaResponse = ""; // TODO
        accountClient.registerAccount(nameEdit.text, passwordEdit.text,
                                      emailEdit.text, captchaResponse);
    }

    Item {
        id: positioner;
        y: parent.height * 0.1;
        x: parent.width * 0.45;

        Text {
            id: serverNameDisplay;
            anchors.horizontalCenter: nameEdit.horizontalCenter;
            text: serverName;
            font.pixelSize: 35;
        }
        LineEdit {
            id: nameEdit;
            width: window.width * 0.5;
            anchors.top: serverNameDisplay.bottom;
            focus: true;
            labelText: qsTr("Username");

            Component.onCompleted: text = settings.value("username", "");

            KeyNavigation.down: passwordEdit;
            backtabTarget: loginButton;
        }
        LineEdit {
            id: emailEdit;
            width: nameEdit.width;
            anchors.top: nameEdit.bottom;
            labelText: qsTr("Email");
            opacity: 0;
            height: 0;

            KeyNavigation.down: passwordEdit;
            KeyNavigation.up: nameEdit;
        }
        LineEdit {
            id: passwordEdit;
            width: nameEdit.width;
            anchors.top: emailEdit.bottom;
            labelText: qsTr("Password");
            echoMode: TextInput.Password;

            KeyNavigation.up: nameEdit;
            KeyNavigation.down: loginButton;
        }
        LineEdit {
            id: passwordConfirmEdit;
            width: nameEdit.width;
            anchors.top: passwordEdit.bottom;
            labelText: qsTr("Password");
            echoMode: TextInput.Password;
            opacity: 0;
            height: 0;

            KeyNavigation.up: passwordEdit;
            KeyNavigation.down: loginButton;
            tabTarget: cancelButton;
        }
        Item {
            id: buttons;
            width: loginButton.width + loginButton.x;
            anchors.top: passwordConfirmEdit.bottom;
            anchors.topMargin: 20;
            anchors.horizontalCenter: passwordEdit.horizontalCenter;

            Button {
                id: cancelButton;
                text: qsTr("Cancel");
                opacity: 0;
                onClicked: loginPage.state = "";
                KeyNavigation.up: passwordConfirmEdit;
                KeyNavigation.right: loginButton;
            }
            Button {
                id: loginButton;
                text: qsTr("Login");
                anchors.left: parent.left;

                KeyNavigation.up: passwordEdit;
                tabTarget: nameEdit;

                enabled: accountClient.state == AccountClient.Connected &&
                         !loggingIn && !loggedIn;

                onClicked: login();
            }
        }
        ErrorLabel {
            id: errorLabel;
            anchors.top: buttons.bottom;
            anchors.horizontalCenter: nameEdit.horizontalCenter;
        }
    }

    function registerOrLogin() {
        if (loginPage.state == "register")
            register();
        else
            login();
    }

    Keys.onReturnPressed: registerOrLogin();
    Keys.onEnterPressed: registerOrLogin();

    Connections {
        target: accountClient;
        onLoginFailed: errorLabel.showError(errorMessage);
        onRegistrationFailed: errorLabel.showError(errorMessage);
    }

    Image {
        id: registrationBar;
        source: "images/bottombar.png";
        width: parent.width;
        y: parent.height;

        TextShadow { target: questionText; }
        Text {
            id: questionText;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.verticalCenterOffset: 2;
            x: 20;
            text: qsTr("Don't have an account yet?");
            font.pixelSize: 35;
            color: "white";
        }

        Button {
            id: registerButton;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.verticalCenterOffset: 2;
            anchors.right: parent.right;
            anchors.rightMargin: 20;
            text: qsTr("Sign Up");
            onClicked: loginPage.state = "register";
            enabled: accountClient.state == AccountClient.Connected &&
                     !loggingIn && !loggedIn && accountClient.registrationAllowed;
        }

        states: [
            State {
                name: "visible";
                when: registerButton.enabled && loginPage.state != "register";
                PropertyChanges {
                    target: registrationBar;
                    y: parent.height - height;
                }
            }
        ]
        transitions: [
            Transition {
                NumberAnimation {
                    property: "y";
                    easing.type: Easing.OutQuad;
                }
            }
        ]
    }

    states: [
        State {
            name: "register"
            PropertyChanges { target: nameEdit; KeyNavigation.down: emailEdit; }
            PropertyChanges { target: emailEdit; opacity: 1; height: 70; }
            PropertyChanges {
                target: passwordEdit;
                KeyNavigation.up: emailEdit;
                KeyNavigation.down: passwordConfirmEdit;
            }
            PropertyChanges { target: passwordConfirmEdit; opacity: 1; height: 70; }
            PropertyChanges { target: cancelButton; opacity: 1; }
            AnchorChanges { target: loginButton; anchors.left: cancelButton.right; }
            PropertyChanges {
                target: loginButton;
                text: qsTr("Register");
                anchors.leftMargin: 10;
                KeyNavigation.left: cancelButton;
                KeyNavigation.up: passwordConfirmEdit;
                onClicked: register();
            }
            PropertyChanges { target: positioner; y: parent.height * 0.065; }
        }
    ]

    transitions: [
        Transition {
            to: "register";
            PropertyAction { property: "text"; }
            NumberAnimation { properties: "opacity"; easing.type: Easing.InQuint; }
            NumberAnimation { properties: "height,y,leftMargin"; easing.type: Easing.InOutQuad; }
            AnchorAnimation { easing.type: Easing.InOutQuad; }
        },
        Transition {
            to: "";
            PropertyAction { property: "text"; }
            NumberAnimation { properties: "opacity"; easing.type: Easing.OutQuart; }
            NumberAnimation { properties: "height,y,leftMargin"; easing.type: Easing.InOutQuad; }
            AnchorAnimation { easing.type: Easing.InOutQuad; }
        }
    ]
}
