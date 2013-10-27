import QtQuick 2.0

/*
 * Helper component for adding a shadow to another text element.
 */
Text {
    property Text target;

    property real offsetX: 1
    property real offsetY: 1

    text: target.text;
    textFormat: target.textFormat
    color: "black";
    x: target.x + offsetX
    y: target.y + offsetY
    width: target.width
    font: target.font;
    opacity: target.opacity;
    visible: target.visible
    wrapMode: target.wrapMode
}
