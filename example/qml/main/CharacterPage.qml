import QtQuick 2.0
import Mana 1.0

Item {
    Text {
        x: 10; y: x;
        color: "beige"
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

        anchors.bottom: title.top
        anchors.bottomMargin: 20;
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Component.onCompleted: characterList.forceActiveFocus();

    PathView {
        id: characterList

        model: accountClient.characterListModel

        width: parent.width / scale
        height: parent.height / scale
        scale: backgroundScale
        transformOrigin: Item.TopLeft

        dragMargin: height / 2
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        highlightRangeMode: PathView.StrictlyEnforceRange;

        focus: true
        Keys.onLeftPressed: decrementCurrentIndex()
        Keys.onRightPressed: incrementCurrentIndex()

        path: Path {
            id: path;

            startX: characterList.width / 2
            startY: characterList.height / 2 + 12

            PathAttribute { name: "characterScale"; value: 1 }
            PathQuad {
                x: path.startX - 20; y: path.startY - 35
                controlX: path.startX + 100; controlY: path.startY - 25
            }
            PathPercent { value: 0.5 }
            PathAttribute { name: "characterScale"; value: 0.6 }
            PathQuad {
                x: path.startX; y: path.startY
                controlX: path.startX - 100; controlY: path.startY - 25
            }
            PathAttribute { name: "characterScale"; value: 1 }
        }

        delegate: Item {
            scale: PathView.characterScale
            z: PathView.characterScale

            CompoundSprite {
                id: sprite
                sprites: model.character.spriteListModel;
                action: "stand";
                direction: Action.DIRECTION_DOWN;
            }

            TextShadow { target: name }
            Text {
                id: name;
                text: model.character.name;
                color: "beige"
                anchors.top: parent.bottom
                anchors.topMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 12
            }
        }
    }

    Text {
        id: title
        anchors.top: parent.top
        anchors.topMargin: parent.height / 10
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Choose your character...")
        color: "beige"
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height / 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Button {
            text: qsTr("New");
            onClicked: window.state = "createCharacter";
            enabled: characterList.count < accountClient.maxCharacters;
        }
        Button {
            text: qsTr("Play")
            onClicked: chooseCharacter()
            enabled: characterList.currentIndex >= 0
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

    Connections {
        target: accountClient;
        onCharacterDataReceived: {
            if (accountClient.connected && characterList.count == 0)
                window.state = "createCharacter";
        }
    }
}
