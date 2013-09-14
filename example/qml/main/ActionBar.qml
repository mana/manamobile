import QtQuick 2.0
import QtQuick.Controls 1.0
import Mana 1.0

Item {
    id: actionBar;

    property int selectedButton: -1;

    function selectedAbility() {
        if (selectedButton != -1)
            return selectedButton;

        return false;
    }

    function reset() {
        selectedButton = -1;
    }

    width: gameClient.abilityListModel.count * (64 + 10);
    height: 32;

    Row {
        spacing: 10;

        Repeater {
            model: gameClient.abilityListModel;

            Button {
                property variant abilityInfo: abilityDB.getInfo(model.ability.id);
                width: 64;
                text: abilityInfo.name
                scale: selectedButton === model.index ? 1.5 : 1;
                onClicked: {
                    if (abilityInfo.targetType === AbilityInfo.TARGET_DIRECTION) {
                        gameClient.useAbilityOnDirection(model.ability.id);
                    } else {
                        if (selectedButton === model.ability.id)
                            selectedButton = -1;
                        else
                            selectedButton = model.ability.id;
                    }
                }
            }
        }
    }
}
