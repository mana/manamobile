import QtQuick 2.0
import Mana 1.0

Rectangle {
    id: questPanel;
    anchors.top: parent.top;
    anchors.bottom: parent.bottom;
    width: parent.width;

    x: state === "closed" ? width : 0;

    state: "closed";

    color: "white";

    function toggle() {
        state = state === "closed" ? "open" : "closed";
    }

    ListView {
        anchors.fill: parent;

        model: gameClient.questlogListModel;

        delegate: Item {
            height: summary.height + description.height;
            Text {
                id: summary;
                color: model.quest.state === Quest.OPEN ? "green" : "black";
                text: "<b>" + model.quest.title + "</b>";
            }
            Text {
                id: description;
                x: 30;
                anchors.top: summary.bottom;
                text: model.quest.description;
            }
        }
    }

    states: [
        State {
            name: "open";
            PropertyChanges {
                target: questPanel;
                opacity: 1;
            }
        },
        State {
            name: "closed";
            PropertyChanges {
                target: questPanel;
                opacity: 0
            }
        }
    ]
}
