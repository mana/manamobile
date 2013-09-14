import QtQuick 2.0
import QtQuick.Controls 1.0
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

    width: gameClient.abilityListModel.count * (64 + 10);
    height: 32;

    Row {
        spacing: 10;

        Repeater {
            model: gameClient.abilityListModel;

            Button {
                property variant abilityInfo: abilityDB.getInfo(model.ability.id);
                width: 32;
                height: 32;
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
                Image {
                    anchors.fill: parent;
                    source: resourceManager.dataUrl + "/" + abilityInfo.icon;
                }
            }
        }
    }
}
