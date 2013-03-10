import QtQuick 2.0

ListModel {
    id: listmodel;

    property bool notfound: false

    Component.onCompleted: {
        var xhr = new XMLHttpRequest;
        var serverlistPath = customServerListPath == "" ?
                    "http://www.manasource.org/serverlist.json": customServerListPath;
        xhr.open("GET", serverlistPath);
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                try {
                    var serverlist = JSON.parse(xhr.responseText);
                } catch (e) {
                    listmodel.notfound = true;
                    return;
                }

                for (var serverIndex in serverlist.servers) {
                    var server = serverlist.servers[serverIndex];
                    listmodel.append({
                                         name: server.name,
                                         hostname: server.hostname,
                                         port: server.port,
                                         description: server.description
                                     });
                }
            }
        }
        xhr.send();
    }
}
