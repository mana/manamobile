import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

/**
 * A TextField in Mana style
 */
TextField {
    anchors.margins: 5;

    style: TextFieldStyle {
        background: BorderImage {
            source: "images/lineedit.png"
            border.bottom: 20;
            border.top: 20;
            border.right: 20;
            border.left: 20;
        }

        font.pixelSize: (parent.height - 10) * 0.7;

        padding.right: 12
        padding.left: 12
    }
}
