import Qt 4.7

Item {
    Text {
        x: 10; y: x;
        text: {
            if (accountClient.username != "")
                "Logged in as " + accountClient.username;
            else
                "Not logged in!";
        }
    }

    ErrorLabel {
        id: errorLabel;

        anchors.bottom: column.top;
        anchors.bottomMargin: 20;
        anchors.left: column.left;
    }

    Column {
        id: column;
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10
        x: parent.width * 0.5

        Text {
            id: title;
            text: qsTr("Choose your character...")
        }

        ListView {
            id: characterList;

            width: window.width * 0.3;
            height: window.height * 0.3;
            model: accountClient.characterListModel;
            delegate: Text {
                text: model.name + " (money: " + model.money + ", level: "
                      + model.level + ")";

                MouseArea {
                    anchors.fill: parent
                    onClicked:  {
                        if (window.characterChosen)
                            return;

                        window.characterChosen = true;
                        errorLabel.clear()
                        accountClient.chooseCharacter(model.index);
                    }
                }
            }

            Rectangle {
                anchors.fill: parent;
                color: "transparent";
                border.color: "black";
            }
        }
    }

    Connections {
        target: accountClient;
        onChooseCharacterFailed: {
            window.characterChosen = false;
            errorLabel.showError(errorMessage);
        }
    }
}
