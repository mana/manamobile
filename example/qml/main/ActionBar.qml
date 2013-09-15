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

    width: 100 + 16 + 32;
    height: width;

    PathView {
        model: abilityDB.isLoaded ? gameClient.abilityListModel : 0;
        onCountChanged: positionViewAtIndex(0, PathView.Beginning);
        interactive: false;
        path: Path {
            id: path;
            startX: actionBar.width - 32;
            startY: actionBar.height - 32;
            PathAttribute {
                name: "elementSize";
                value: 2;
            }
            PathLine {
                x: path.startX - 100;
                y: path.startY;
            }
            PathAttribute {
                name: "elementSize";
                value: 1;
            }
            PathPercent {
                value: 0.0001;
            }
            PathArc {
                relativeX: 100;
                relativeY: -100;
                radiusX: 100;
                radiusY: 100;
            }
            PathAttribute {
                name: "elementSize";
                value: 1;
            }
            PathPercent {
                value: 1;
            }
        }

        delegate: Item {
            scale: PathView.elementSize;
            ImageButton {
                anchors.centerIn: parent;
                property variant abilityInfo: abilityDB.getInfo(model.ability.id);
                onClicked: {
                    if (abilityInfo.targetType === AbilityInfo.TARGET_DIRECTION) {
                        gameClient.useAbilityOnDirection(model.ability.id);
                    } else {
                        if (selectedAbilityId === model.ability.id)
                            selectedAbilityId = -1;
                        else
                            selectedAbilityId = model.ability.id;
                    }
                }

                imagePath: resourceManager.dataUrl + "/" + abilityInfo.icon;
            }
        }
    }

}
