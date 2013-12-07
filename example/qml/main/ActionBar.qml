import QtQuick 2.0
import Mana 1.0

Item {
    id: actionBar;

    property int selectedAbilityId: -1;

    function selectedAbility() {
        if (selectedAbilityId != -1)
            return selectedAbilityId;

        return false;
    }

    function reset() {
        selectedAbilityId = -1;
    }

    width: 120
    height: 120

    PathView {
        model: abilityDB.isLoaded ? gameClient.abilityListModel : 0;
        onCountChanged: positionViewAtIndex(0, PathView.Beginning);
        interactive: false;
        path: Path {
            id: path;

            readonly property real radius: 60
            readonly property real startAngle: 220 * Math.PI / 180
            readonly property real endAngle: 50 * Math.PI / 180

            startX: actionBar.width - 32;
            startY: actionBar.height - 32;
            PathLine {
                x: path.startX + Math.cos(path.startAngle) * path.radius
                y: path.startY - Math.sin(path.startAngle) * path.radius
            }
            PathPercent {
                value: 0.0001;
            }
            PathArc {
                x: path.startX + Math.cos(path.endAngle) * path.radius
                y: path.startY - Math.sin(path.endAngle) * path.radius
                radiusX: path.radius
                radiusY: path.radius
            }
            PathPercent {
                value: 1;
            }
        }

        delegate: Item {
            ImageButton {
                anchors.centerIn: parent;
                baseName: index == 0 ? "images/bigroundbutton" : "images/roundbutton"

                property variant abilityInfo: abilityDB.getInfo(model.ability.id);

                function faceNearestMonster()
                {
                    var closestBeing = gameClient.beingListModel.closestBeingAround(gameClient.player);
                    gameClient.lookAt(closestBeing.x, closestBeing.y);
                }

                onPressedChanged: {
                    if (!pressed)
                        return;

                    if (abilityInfo.targetType === AbilityInfo.TARGET_DIRECTION) {
                        faceNearestMonster();
                        gameClient.useAbilityOnDirection(model.ability.id);
                    } else {
                        if (selectedAbilityId === model.ability.id)
                            selectedAbilityId = -1;
                        else
                            selectedAbilityId = model.ability.id;
                    }
                }

                imagePath: resourceManager.dataUrl + abilityInfo.icon;
            }
        }
    }
}
