import QtQuick 2.0

Rectangle {
    id: statusPanel;

    state: "closed";
    color: "BurlyWood";

    width: parent.width / 2;

    anchors.top: parent.top;
    anchors.bottom: parent.bottom;


    function toggle() {
        state = state === "closed" ? "open" : "closed";
    }

    ListView {
        anchors.fill: parent;
        anchors.topMargin: 5;
        model: attributeDB.isLoaded ? gameClient.attributeListModel : null;
        delegate: Item {
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            height: attributeName.height;

            Text {
                id: attributeName;
                text: {
                    var attributeInfo = attributeDB.getInfo(model.value.attributeId);
                    return attributeInfo.name + " " + model.value.modified;
                }
            }
            Text {
                property real diff: model.value.modified - model.value.base;
                text: diff > 0 ? "(+" + diff + ")" : "(" + diff + ")";
                visible: diff != 0;
                color: diff > 0 ? "green" : "red";
                anchors.left: parent.right;
                anchors.leftMargin: -100;
            }
        }
    }

    Rectangle {
        width: 20;
        color: "black";

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.right;

        MouseArea {
            id: handle;

            anchors.fill: parent;

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
        }

        Text {
            text: qsTr("Status");
            color: "white";
            rotation: -90;
            font.bold: true;
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
