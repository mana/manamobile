import QtQuick 2.0

/**
 * A big red label which animates when showError() is called on it. Use clear()
 * to unset the error again.
 */
Text {
    id: errorLabel

    color: "red"
    font.bold: true

    function showError(message) {
        errorLabel.text = message;
        scaleAnimation.running = true;
    }

    function clear() {
        errorLabel.text = "";
    }

    ParallelAnimation {
        id: scaleAnimation
        NumberAnimation {
            target: errorLabel;
            property: "scale"; from: 2; to: 1;
            easing.type: Easing.OutQuad
        }
        NumberAnimation {
            target: errorLabel;
            property: "opacity"; from: 0; to: 1;
            easing.type: Easing.OutQuad
        }
    }
}
