import QtQuick 2.0

Item {
    anchors.horizontalCenter: parent.horizontalCenter

    property alias text: titleLabel.text

    BorderImage {
        id: titleBackground
        source: "images/scroll_header.png"
        anchors.left: titleLabel.left
        anchors.right: titleLabel.right
        anchors.margins: -12
        border.right: 8
        border.left: 8
        y: 2
        smooth: false
    }

    Text {
        id: titleLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: titleBackground.verticalCenter
        color: "#d2b777"
        font.pixelSize: 10
        font.bold: true
        font.capitalization: Font.SmallCaps
    }
}
