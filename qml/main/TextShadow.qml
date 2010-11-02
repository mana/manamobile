import Qt 4.7

/*
 * Helper component for adding a shadow to another text element.
 */
Text {
    property Text target;

    text: target.text;
    color: "black";
    x: target.x + 1;
    y: target.y + 1;
    font: target.font;
}
