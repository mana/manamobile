import QtQuick 2.0

Item {
    property alias name: nameLabel.text
    property alias value: valueLabel.text

    width: 110
    height: 14

    Rectangle {
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "#C39753"
    }
    Rectangle {
        id: valueLabelBackground
        height: 1
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 30
        border.color: "#C39753"
        border.width: 1
        color: "transparent"
    }
    Text {
        id: nameLabel
        font.pixelSize: 10
        color: "#3F2B25"
        elide: Text.ElideRight
        anchors.left: parent.left
        anchors.right: valueLabelBackground.left
    }
    Text {
        id: valueLabel
        anchors.centerIn: valueLabelBackground
        color: "#3F2B25"
        font.pixelSize: 12
        scale: Math.min(1, (valueLabelBackground.width - 4) / contentWidth)
    }
}
