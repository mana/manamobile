import QtQuick 2.0
import Mana 1.0

Item {
    readonly property int numHairStyles: hairDB.hairs.length
    property int hairIndex: 0

    function nextHairStyle() {
        hairIndex = (hairIndex + 1) % numHairStyles;
    }

    function previousHairStyle() {
        var index = hairIndex - 1;
        if (index < 0)
            index = numHairStyles - 1;
        hairIndex = index;
    }

    function createCharacter() {
        var name = nameEdit.text;
        if (name === "")
            return;

        errorLabel.clear();
        accountClient.createCharacter(name,
                                      character.gender,
                                      character.hairStyle,
                                      character.hairColor,
                                      [5, 5, 5, 5, 5, 5]);
    }

    ErrorLabel {
        id: errorLabel;
        anchors.top: parent.top;
        anchors.topMargin: 20;
        anchors.horizontalCenter: parent.horizontalCenter;
    }

    Character {
        id: character
        gender: Being.GENDER_MALE
        hairStyle: {
            var style = hairDB.hairs[hairIndex];
            return style ? style.id : 0;
        }
        hairColor: 1
    }

    CompoundSprite {
        id: characterPreview
        sprites: character.spriteListModel
        action: "stand"
        direction: Action.DIRECTION_DOWN
        scale: 2

        anchors.centerIn: parent
        anchors.verticalCenterOffset: maxHeight / 2
    }

    LineEdit {
        id: nameEdit
        placeholderText: qsTr("Name")
        focus: true
        width: 300
        anchors.horizontalCenter: characterPreview.horizontalCenter
        anchors.bottom: parent.bottom
        onAccepted: createCharacter()
    }

    Button {
        text: qsTr(">")
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        onClicked: nextHairStyle()
    }

    Button {
        text: qsTr("<")
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        onClicked: previousHairStyle()
    }

    Button {
        text: qsTr("Back")
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        onClicked: window.state = "chooseCharacter"
    }

    Button {
        text: qsTr("Create")
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        enabled: nameEdit.text != ""
        onClicked: createCharacter()
    }

    Connections {
        target: accountClient
        onCreateCharacterSucceeded: window.state = "chooseCharacter"
        onCreateCharacterFailed: errorLabel.showError(errorMessage)
    }

    Component.onCompleted: {
        nameEdit.focus = true;
    }
}
