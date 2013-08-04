import QtQuick 2.0

/**
 * An item that indicates something is happening.
 */
Image {
    id: indicator

    property bool running: false

    source: "images/progress-indicator.png"
    opacity: 0
    scale: 4
    smooth: true;

    NumberAnimation {
        id: rotateAnimation
        target: indicator
        property: "rotation"
        from: 0
        to: 360
        duration: 1000
        loops: Animation.Infinite
    }

    states: [
        State {
            name: "running"
            when: indicator.running
            PropertyChanges {
                target: rotateAnimation
                running: true
            }
            PropertyChanges {
                target: indicator
                opacity: 1
                scale: 1
            }
        }
    ]

    transitions: [
        Transition {
            from: "running"
            SequentialAnimation {
                NumberAnimation {
                    properties: "opacity,scale"
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    property: "running"
                }
            }
        },
        Transition {
            to: "running"
            SequentialAnimation {
                NumberAnimation {
                    properties: "opacity,scale"
                    duration: 1000
                    easing.type: Easing.InOutQuad
                }
            }
        }
    ]
}
