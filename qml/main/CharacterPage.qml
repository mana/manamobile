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

    function chooseCharacter() {
        if (window.characterChosen)
            return;
        if (characterList.currentIndex < 0)
            return;

        window.characterChosen = true;
        errorLabel.clear()
        accountClient.chooseCharacter(characterList.currentIndex);
    }

    Keys.onReturnPressed: chooseCharacter();
    Keys.onEnterPressed: chooseCharacter();

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
            focus: window.state == "chooseCharacter";

            width: window.width * 0.3;
            height: window.height * 0.3;
            model: accountClient.characterListModel;
            delegate: Text {
                text: model.name + " (money: " + model.money + ", level: "
                      + model.level + ")";

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        characterList.currentIndex = model.index;
                        chooseCharacter();
                    }
                }
            }
            highlight: Rectangle {
                color: "black";
                opacity: 0.2;
                width: characterList.width;
            }

            onCountChanged: {
                // Select first item when possible
                if (currentIndex == -1 && count > 0)
                    currentIndex = 0;
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
