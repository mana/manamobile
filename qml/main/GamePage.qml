import Qt 4.7

Rectangle {
    width: 640;
    height: 480;

    color: "black";

    Text {
        color: "white";
        anchors.centerIn: parent;
        text: "Current map: " + gameClient.currentMap;
    }
}
