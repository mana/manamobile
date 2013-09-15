import QtQuick 2.0

Item {
    id: statusPanel;

    Image {
        id: tab
        source: "images/tab.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.right
        anchors.leftMargin: -3
        smooth: false
    }
    Image {
        source: "images/tab_icon_character.png"
        anchors.centerIn: tab
        smooth: false
    }
    MouseArea {
        id: handle;

        anchors.fill: tab
        anchors.margins: -5

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

    BorderImage {
        anchors.fill: parent
        anchors.leftMargin: -33;
        anchors.rightMargin: -1

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
        anchors.rightMargin: 25

        clip: true

        ListView {
            anchors.fill: parent;
            anchors.leftMargin: 5
            anchors.rightMargin: 5

            topMargin: 5
            bottomMargin: 5

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
                    anchors.leftMargin: -50;
                }
            }
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
