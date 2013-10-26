import QtQuick 2.0
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
        Qt.inputMethod.hide();
    }

    Item {
        id: visibleArea

        anchors.fill: parent
        anchors.bottomMargin: Qt.inputMethod.visible ? 316 : registrationBar.height

        Behavior on anchors.bottomMargin {
            NumberAnimation {
                easing.type: Easing.OutCubic
            }
        }
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

    Image {
        source: "images/sourceoftales.png"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: positioner.top

        anchors.margins: 30
        fillMode: Image.PreserveAspectFit
    }

    Item {
        id: positioner;
        anchors.centerIn: visibleArea
        width: Math.min(window.width - 100, 400);
        height: childrenRect.height

        LineEdit {
            id: nameEdit;
            anchors.left: parent.left
            anchors.right: parent.right
            focus: true;
            placeholderText: qsTr("Username");
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase;
            maximumLength: accountClient.maximumNameLength;

            Component.onCompleted: {
                text = settings.value("username", "");
                if (text !== "")
                    passwordEdit.focus = true;
            }

            KeyNavigation.down: passwordEdit;
        }
        LineEdit {
            id: emailEdit;
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: nameEdit.bottom;
            placeholderText: qsTr("Email");
            visible: false;
            opacity: 0;
            height: 0;
            inputMethodHints: Qt.ImhEmailCharactersOnly;

            KeyNavigation.down: passwordEdit;
            KeyNavigation.up: nameEdit;
        }
        LineEdit {
            id: passwordEdit;
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: emailEdit.bottom;
            placeholderText: qsTr("Password");
            echoMode: TextInput.Password;
            inputMethodHints: Qt.ImhHiddenText | Qt.ImhSensitiveData |
                              Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText |
                              Qt.ImhPreferLowercase;

            KeyNavigation.up: nameEdit;
            KeyNavigation.down: loginButton;
        }
        LineEdit {
            id: passwordConfirmEdit;
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: passwordEdit.bottom;
            placeholderText: qsTr("Password");
            echoMode: TextInput.Password;
            inputMethodHints: passwordEdit.inputMethodHints;
            visible: false;
            opacity: 0;
            height: 0;

            KeyNavigation.up: passwordEdit;
            KeyNavigation.down: loginButton;
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
                visible: false;
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

    Keys.onReturnPressed: {
        if (event.modifiers ===  Qt.NoModifier)
            registerOrLogin();
        else
            event.accepted = false;
    }
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
        visible: false;

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
                    visible: true;
                }
            }
        ]
        transitions: [
            Transition {
                to: ""
                SequentialAnimation {
                    NumberAnimation { property: "y"; easing.type: Easing.OutQuad; }
                    PropertyAction { property: "visible"; }
                }
            },
            Transition {
                to: "visible"
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
            PropertyChanges { target: emailEdit; visible: true; opacity: 1; height: emailEdit.implicitHeight; }
            PropertyChanges {
                target: passwordEdit;
                KeyNavigation.up: emailEdit;
                KeyNavigation.down: passwordConfirmEdit;
            }
            PropertyChanges { target: passwordConfirmEdit; visible: true; opacity: 1; height: passwordConfirmEdit.implicitHeight; }
            PropertyChanges { target: cancelButton; visible: true; opacity: 1; }
            AnchorChanges { target: loginButton; anchors.left: cancelButton.right; }
            PropertyChanges {
                target: loginButton;
                text: qsTr("Register");
                anchors.leftMargin: 10;
                KeyNavigation.left: cancelButton;
                KeyNavigation.up: passwordConfirmEdit;
                onClicked: register();
            }
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
            SequentialAnimation {
                ParallelAnimation {
                    PropertyAction { property: "text"; }
                    NumberAnimation { properties: "opacity"; easing.type: Easing.OutQuart; }
                    NumberAnimation { properties: "height,y,leftMargin"; easing.type: Easing.InOutQuad; }
                    AnchorAnimation { easing.type: Easing.InOutQuad; }
                }
                PropertyAction { property: "visible"; }
            }
        }
    ]
}
