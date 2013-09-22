import QtQuick 2.0
import Mana 1.0

Button {
    id: respawnButton
    text: "Respawn"
    opacity: 0
    visible: opacity > 0
    scale: 0.8

    onClicked: {
        gameClient.respawn();
        respawnButton.state = "";
    }

    states: [
        State {
            name: "visible"
            when: gameClient.player && gameClient.player.action === "dead"
            PropertyChanges {
                target: respawnButton
                opacity: 1
                scale: 1
            }
        }
    ]

    transitions: [
        Transition {
            to: "visible"
            SequentialAnimation {
                PauseAnimation { duration: 5 * 75 + 500 }
                ParallelAnimation {
                    NumberAnimation {
                        property: "opacity"
                        duration: 400
                        easing.type: Easing.OutQuad
                    }
                    NumberAnimation {
                        property: "scale"
                        duration: 400
                        easing.type: Easing.OutBack
                    }
                }
            }
        },
        Transition {
            from: "visible"
            NumberAnimation {
                properties: "opacity,scale"
                duration: 400
                easing.type: Easing.OutQuad
            }
        }
    ]
}
