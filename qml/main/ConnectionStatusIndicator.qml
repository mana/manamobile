import QtQuick 2.0
import Mana 1.0

Rectangle {
    property variant client;

    width: 10;
    height: 10;
    radius: 5;

    color: {
        var state = client.state;
        if (state === ENetClient.Connected)
            return "green";
        else if (state === ENetClient.Disconnected)
            return "red";

        return "orange";
    }
}
