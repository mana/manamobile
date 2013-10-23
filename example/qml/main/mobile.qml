import QtQuick 2.0
import QtQuick.Window 2.0

/**
 * This is the mobile version of the QML based Mana client.
 */
Client {
    id: client

    width: 1280
    height: 720

    contentOrientation: {
        if (Screen.orientation == Qt.InvertedLandscapeOrientation)
            Qt.InvertedLandscapeOrientation;
        else
            Qt.LandscapeOrientation;
    }

    MainWindow { anchors.fill: parent; }
}
