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
        model: loginManager.characterListModel;
        delegate: Text {
            text: model.name + " (money: " + model.money + ", level: "
                  + model.level + ")";
        }

        Rectangle {
            anchors.fill: parent;
            color: "transparent";
            border.color: "black";
        }
    }
}
