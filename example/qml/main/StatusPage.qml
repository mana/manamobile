import QtQuick 2.0

Item {
    id: statusPanel;

    BorderImage {
        anchors.fill: parent
        anchors.leftMargin: -33;
        anchors.rightMargin: -22;

        source: "images/scroll_medium_horizontal.png"
        border.left: 40; border.top: 31
        border.right: 38; border.bottom: 32
        smooth: false
    }

    state: "closed";

    width: parent.width / 2 - 22 - 32;

    anchors.top: parent.top;
    anchors.bottom: parent.bottom;


    function toggle() {
        state = state === "closed" ? "open" : "closed";
    }

    Item {
        id: contents
        anchors.fill: parent
        anchors.topMargin: 12
        anchors.bottomMargin: 7
        clip: true

        ListView {
            anchors.fill: parent;
            leftMargin: 5
            topMargin: 5
            bottomMargin: 5
            rightMargin: 5
            model: attributeDB.isLoaded ? gameClient.attributeListModel : null;
            delegate: Item {
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: attributeName.height;

                Text {
                    id: attributeName;
                    text: {
                        var attributeInfo = attributeDB.getInfo(model.value.attributeId);
                        return attributeInfo.name + " " + model.value.modified;
                    }
                    font.pixelSize: 12
                }
                Text {
                    property real diff: model.value.modified - model.value.base;
                    text: diff > 0 ? "(+" + diff + ")" : "(" + diff + ")";
                    visible: diff != 0;
                    color: diff > 0 ? "green" : "red";
                    font.pixelSize: 12
                    anchors.left: parent.right;
                    anchors.leftMargin: -100;
                }
            }
        }
    }

    MouseArea {
        id: handle;

        width: 20;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.right;

        drag.target: statusPanel;
        drag.axis: Drag.XAxis;
        drag.minimumX: -statusPanel.width;
        drag.maximumX: 0;

        onClicked: toggle();
        onReleased: {
            var open = -statusPanel.x < statusPanel.width / 2;
            statusPanel.x = open ? 0 : -statusPanel.width;
            statusPanel.state = open ? "open" : "closed";
        }

        Text {
            text: qsTr("Status");
            color: "#3f2b25";
            rotation: -90;
            font.bold: true;
            font.pixelSize: 12
            anchors.centerIn: parent;
        }
    }

    Behavior on x {
        NumberAnimation { easing.type: Easing.OutQuad }
    }

    states: [
        State {
            name: "open";

            PropertyChanges {
                target: statusPanel;
                x: 0;
            }
        },
        State {
            name: "closed";
            PropertyChanges {
                target: statusPanel;
                x: -statusPanel.width;
            }
        }
    ]
}
