import QtQuick 2.0
import QtQuick.Window 2.0
import Mana 1.0

Window {
    id: resourcesWindow;
    width: 800;
    height: 800;
    title: "Resources";
    visible: true;

    ConnectionStatus {
        z: 1;
        anchors.top: parent.top;
        anchors.right: parent.right;
    }

    ListView {
        focus: true;

        Keys.onPressed: {
            if (event.key === Qt.Key_F10)
                resourcesWindowLoader.active = !resourcesWindowLoader.active;
        }

        anchors.fill: parent;
        anchors.topMargin: 5;
        anchors.bottomMargin: 5;
        model: resourceManager.resourceListModel;
        delegate: Rectangle {
            color: mouseArea.containsMouse ? "lightgray" : "transparent";
            height: 20;
            anchors.left: parent.left;
            anchors.right: parent.right;
            opacity: resource.referenceCount === 0 ? 0.5 : 1;

            MouseArea {
                id: mouseArea;
                anchors.fill: parent;
                hoverEnabled: true;
                onClicked: Qt.openUrlExternally(resource.url);
                cursorShape: Qt.PointingHandCursor;
            }

            Text {
                text: resource.url;
                anchors.left: parent.left;
                anchors.leftMargin: 5;
                anchors.right: referenceCountLabel.left;
                anchors.rightMargin: 10;
                elide: Text.ElideRight;

                anchors.verticalCenter: parent.verticalCenter;
            }
            Text {
                id: referenceCountLabel;
                text: resource.referenceCount;
                anchors.right: parent.right;
                anchors.rightMargin: 100;
                anchors.verticalCenter: parent.verticalCenter;
            }
            Rectangle {
                id: statusRect;
                anchors.top: parent.top;
                anchors.right: parent.right;
                anchors.bottom: parent.bottom;
                anchors.topMargin: 1;
                anchors.bottomMargin: 1;
                anchors.rightMargin: 5;
                radius: 5;
                width: statusLabel.implicitWidth + 20;
                color: {
                    switch (resource.status) {
                    case Resource.Null:
                        return "gray";
                    case Resource.Ready:
                        return "green";
                    case Resource.Error:
                        return "red";
                    case Resource.Loading:
                        return "orange";
                    }
                }

                Text {
                    id: statusLabel;
                    anchors.centerIn: parent;
                    text: {
                        switch (resource.status) {
                        case Resource.Null:
                            return "Null";
                        case Resource.Ready:
                            return "Ready";
                        case Resource.Error:
                            return "Error";
                        case Resource.Loading:
                            return "Loading";
                        }
                        return "Huh?";
                    }
                    color: "white";
                }
            }
        }
    }
}
