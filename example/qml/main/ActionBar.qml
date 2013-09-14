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

    width: repeater.count * 64;

    Row {
        spacing: 10;

        anchors.bottom: parent.bottom;

        Repeater {
            id: repeater;
            model: abilityDB.isLoaded ? gameClient.abilityListModel : 0;

            ImageButton {
                property variant abilityInfo: abilityDB.getInfo(model.ability.id);
                scale: selectedAbilityId === model.ability.id ? 1.5 : 1;
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
