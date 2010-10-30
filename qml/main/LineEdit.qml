import Qt 4.7

/**
 * A LineEdit in Mana style
 */
FocusScope {
    property alias text: textInput.text
    property alias echoMode: textInput.echoMode
    property variant tabTarget: KeyNavigation.down;
    property variant backtabTarget: KeyNavigation.up;

    height: textInput.height + 4 + 5

    Keys.onTabPressed: if (tabTarget) tabTarget.focus = true;
    Keys.onBacktabPressed: if (backtabTarget) backtabTarget.focus = true;

    onActiveFocusChanged: textInput.selectAll();

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

        selectByMouse: true
        passwordCharacter: "●"
    }
}
