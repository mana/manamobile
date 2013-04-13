import QtQuick 2.0
import QtQuick.Controls 1.0

Item {
    property int selectedButton: -1;

    width: 32 * 10 + 9 * 10;
    height: 32;

    Row {
        spacing: 10;

        Repeater {
            model: 10
            Button {
                width: 32;
                text: "A" + (model.index + 1);
                scale: selectedButton === model.index ? 1.5 : 1;
                onClicked: {
                    if (selectedButton === model.index)
                        selectedButton = -1
                    else
                        selectedButton = model.index
                }
            }
        }
    }
}
