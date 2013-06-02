import QtQuick 2.0
import Mana 1.0

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

    Component.onCompleted: characterList.forceActiveFocus();

    Column {
        id: column;
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10
        x: parent.width * 0.5

        Text {
            id: title;
            text: qsTr("Choose your character...")
        }

        GridView {
            id: characterList;
            flow: GridView.LeftToRight;

            width: window.width * 0.3;
            height: window.height * 0.3;

            clip: true;

            model: accountClient.characterListModel;
            delegate: Item {
                id: container;
                width: 64;
                height: 64 + name.height;

                CompoundSprite {
                    id: sprites;
                    sprites: model.character.spriteListModel;
                    action: "stand";
                    direction: 2;

                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.bottom: parent.bottom;
                    anchors.bottomMargin: name.height;
                }

                Text {
                    id: name;
                    text: model.character.name;
                    anchors.bottom: parent.bottom;
                    anchors.horizontalCenter: parent.horizontalCenter;
                }

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
                width: characterList.currentItem.width;
                height: characterList.currentItem.height;
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
                border.width: 1;
            }
        }
    }

    Connections {
        target: accountClient;
        onChooseCharacterFailed: {
            window.characterChosen = false;
            errorLabel.showError(errorMessage);
            console.log(errorMessage);
        }
    }
}
