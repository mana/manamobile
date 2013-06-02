import QtQuick 2.0

Item {
    width: indicators.width + 4;
    height: indicators.height + 4;

    Rectangle {
        color: "black";
        anchors.fill: parent;
        anchors.topMargin: -5;
        anchors.rightMargin: -5;
        opacity: 0.25;
        radius: 5;
    }

    Row {
        id: indicators;
        spacing: 2;
        x: 2;
        y: 2;

        ConnectionStatusIndicator { client: accountClient; }
        ConnectionStatusIndicator { client: chatClient; }
        ConnectionStatusIndicator { client: gameClient; }
    }
}
