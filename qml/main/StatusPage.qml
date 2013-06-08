import QtQuick 2.0

Rectangle {
    ListView {
        width: parent.width;
        height: parent.height;
        model: attributeDB.isLoaded && gameClient.player
               ? gameClient.attributeListModel : 0;
        delegate: Item {
            property variant attributeInfo: attributeDB.getInfo(model.value.attributeId);
            width: parent.width;
            height: 50;
            Text {
                text: parent.attributeInfo.id + ": "
                      + parent.attributeInfo.name + " "
                      + model.value.modified
                      + " (+" +( model.value.modified - model.value.base) + ")";
            }
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_C)
            gamePage.state = "game";

        event.accepted = true;
    }
}
