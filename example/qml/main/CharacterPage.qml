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

    property bool deletingCharacter: false

    function chooseCharacter() {
        if (window.characterChosen)
            return;
        if (characterList.currentIndex < 0)
            return;

        window.characterChosen = true;
        errorLabel.clear()
        accountClient.chooseCharacter(characterList.currentIndex);
    }

    function deleteCharacter() {
        if (characterList.currentIndex < 0)
            return;

        deletingCharacter = true;
        accountClient.deleteCharacter(characterList.currentIndex);
    }

    Keys.onReturnPressed: {
        if (event.modifiers === Qt.NoModifier)
            chooseCharacter();
        else
            event.accepted = false;
    }
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
            id: characterDelegate

            scale: PathView.characterScale
            z: PathView.characterScale

            property string name: model.character.name

            CompoundSprite {
                id: sprite
                sprites: model.character.spriteListModel;
                action: "stand";
                direction: Action.DIRECTION_DOWN;
            }

            TextShadow { target: nameLabel }
            Text {
                id: nameLabel
                text: characterDelegate.name
                textFormat: Text.PlainText
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
            text: qsTr("Delete")
            onClicked: confirmDialog.visible = true
            enabled: characterList.currentIndex >= 0 && !characterChosen
        }
        Button {
            text: qsTr("New");
            onClicked: window.state = "createCharacter";
            enabled: characterList.count < accountClient.maxCharacters && !characterChosen
        }
        Button {
            text: qsTr("Play")
            onClicked: chooseCharacter()
            enabled: characterList.currentIndex >= 0 && !characterChosen
        }
    }

    MessageDialog {
        id: confirmDialog
        scale: 2
        title: qsTr("Delete Character")
        message: qsTr("Are you sure you want to delete this character? This cannot be undone!")
        confirmText: qsTr("Delete Character")
        visible: false
        onConfirmed: deleteCharacter()
    }

    Connections {
        target: accountClient;
        onChooseCharacterFailed: {
            window.characterChosen = false;
            errorLabel.showError(errorMessage);
            console.log(errorMessage);
        }
        onDeleteCharacterFailed: {
            deletingCharacter = false;
            errorLabel.showError(errorMessage);
            console.log(errorMessage);
        }
        onDeleteCharacterSucceeded: deletingCharacter = false;
    }
}
