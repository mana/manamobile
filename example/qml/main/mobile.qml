import QtQuick 2.0

/**
 * This is the mobile version of the QML based Mana client.
 */
Client {
    id: client

    height: 1280;
    width: 720;

    MainWindow { anchors.fill: parent; }
}
