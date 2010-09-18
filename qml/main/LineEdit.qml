import Qt 4.7

/**
 * A LineEdit in Mana style
 */
FocusScope {
    property alias text: textInput.text
    property alias echoMode: textInput.echoMode

    height: textInput.height + 4 + 5

    BorderImage {
        anchors.fill: parent

        source: "images/deepbox.png"
        border.bottom: 2
        border.top: 3
        border.right: 2
        border.left: 3
    }

    TextInput {
        id: textInput

        y: 5
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 5

        focus: true

        passwordCharacter: "●"
    }
}
