import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    property string imagePath;

    style: ButtonStyle {
        background: Image {
            source: "images/scrollbutton.png"
        }
    }

    Image {
        source: imagePath;
    }
}
