import Qt 4.7

Item {
    Text {
        id: title;
        x: parent.width * 0.5
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("Choose your character...")
    }

    ListView {
        id: characterList;

        x: title.x;
        width: window.width * 0.3;
        height: window.height * 0.3;
        anchors.top: title.bottom;
        anchors.topMargin: 10;
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

    Connections {
        target: accountClient;
        onChooseCharacterFailed: {
            window.characterChosen = false;
        }
    }
}
