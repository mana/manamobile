import QtQuick 2.0

Item {
    id: statusPanel;

    state: "closed"

    function toggle() {
        state = state === "closed" ? "open" : "closed";
    }

    Image {
        id: tab
        source: "images/tab.png"
        y: parent.height / 4
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
            statusPanel.state = ""  // hack to make sure to trigger transition
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

    transitions: [
        Transition {
            NumberAnimation { property: "x"; easing.type: Easing.OutQuad }
        }
    ]
}
