import QtQuick 2.0

/**
 * This is the mobile version of the QML based Mana client.
 */
Client {
    id: client

    width: 1280
    height: 720

    contentOrientation: Qt.LandscapeOrientation

    MainWindow { anchors.fill: parent; }
}
