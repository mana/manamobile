import qbs 1.0

DynamicLibrary {
    name: "libmana"
    destinationDirectory: "Mana"
    targetName: "mana"

    Depends {
        name: "Qt"
        submodules: [
            "network",
            "quick",
        ]
    }

    Depends {
        name: "cpp"
    }

    Group {
        name: "Binaries"
        qbs.install: true
        qbs.installDir: "lib/libmana/qml/Mana"
        fileTagsFilter: "dynamiclibrary"
    }

    Group {
        name: "qmldir label"
        qbs.install: true
        qbs.installDir: "lib/libmana/qml/Mana"
        files: ["src/mana/qml/Mana/qmldir"]
    }

    Group {
        name: "Manamobile C++ code"
        files: [
            "mana/abilitylistmodel.cpp",
            "mana/abilitylistmodel.h",
            "mana/accountclient.cpp",
            "mana/accountclient.h",
            "mana/attributelistmodel.cpp",
            "mana/attributelistmodel.h",
            "mana/being.cpp",
            "mana/being.h",
            "mana/beinglistmodel.cpp",
            "mana/beinglistmodel.h",
            "mana/character.cpp",
            "mana/character.h",
            "mana/characterlistmodel.cpp",
            "mana/characterlistmodel.h",
            "mana/chatclient.cpp",
            "mana/chatclient.h",
            "mana/collisionhelper.cpp",
            "mana/collisionhelper.h",
            "mana/droplistmodel.cpp",
            "mana/droplistmodel.h",
            "mana/enetclient.cpp",
            "mana/enetclient.h",
            "mana/gameclient.cpp",
            "mana/gameclient.h",
            "mana/inventorylistmodel.cpp",
            "mana/inventorylistmodel.h",
            "mana/logicdriver.cpp",
            "mana/logicdriver.h",
            "mana/manaplugin.cpp",
            "mana/manaplugin.h",
            "mana/mapitem.cpp",
            "mana/mapitem.h",
            "mana/messagein.cpp",
            "mana/messagein.h",
            "mana/messageout.cpp",
            "mana/messageout.h",
            "mana/monster.cpp",
            "mana/monster.h",
            "mana/npc.cpp",
            "mana/npc.h",
            "mana/protocol.h",
            "mana/questloglistmodel.cpp",
            "mana/questloglistmodel.h",
            "mana/resource/abilitydb.cpp",
            "mana/resource/abilitydb.h",
            "mana/resource/action.cpp",
            "mana/resource/action.h",
            "mana/resource/animation.cpp",
            "mana/resource/animation.h",
            "mana/resource/attributedb.cpp",
            "mana/resource/attributedb.h",
            "mana/resource/hairdb.cpp",
            "mana/resource/hairdb.h",
            "mana/resource/imageresource.cpp",
            "mana/resource/imageresource.h",
            "mana/resource/imageset.cpp",
            "mana/resource/imageset.h",
            "mana/resource/itemdb.cpp",
            "mana/resource/itemdb.h",
            "mana/resourcelistmodel.cpp",
            "mana/resourcelistmodel.h",
            "mana/resourcemanager.cpp",
            "mana/resourcemanager.h",
            "mana/resource/mapresource.cpp",
            "mana/resource/mapresource.h",
            "mana/resource/monsterdb.cpp",
            "mana/resource/monsterdb.h",
            "mana/resource/npcdb.cpp",
            "mana/resource/npcdb.h",
            "mana/resource/racedb.cpp",
            "mana/resource/racedb.h",
            "mana/resource/resource.cpp",
            "mana/resource/resource.h",
            "mana/resource/spritedef.cpp",
            "mana/resource/spritedef.h",
            "mana/settings.cpp",
            "mana/settings.h",
            "mana/shoplistmodel.cpp",
            "mana/shoplistmodel.h",
            "mana/spriteitem.cpp",
            "mana/spriteitem.h",
            "mana/spritelistmodel.cpp",
            "mana/spritelistmodel.h",
            "mana/tilelayeritem.cpp",
            "mana/tilelayeritem.h",
            "mana/xmlreader.h",
            "safeassert.h",
            "tiled/compression.cpp",
            "tiled/compression.h",
            "tiled/gidmapper.cpp",
            "tiled/gidmapper.h",
            "tiled/imagelayer.cpp",
            "tiled/imagelayer.h",
            "tiled/isometricrenderer.cpp",
            "tiled/isometricrenderer.h",
            "tiled/layer.cpp",
            "tiled/layer.h",
            "tiled/logginginterface.h",
            "tiled/map.cpp",
            "tiled/map.h",
            "tiled/mapobject.cpp",
            "tiled/mapobject.h",
            "tiled/mapreader.cpp",
            "tiled/mapreader.h",
            "tiled/mapreaderinterface.h",
            "tiled/maprenderer.cpp",
            "tiled/maprenderer.h",
            "tiled/mapwriter.cpp",
            "tiled/mapwriter.h",
            "tiled/mapwriterinterface.h",
            "tiled/objectgroup.cpp",
            "tiled/objectgroup.h",
            "tiled/object.h",
            "tiled/orthogonalrenderer.cpp",
            "tiled/orthogonalrenderer.h",
            "tiled/properties.cpp",
            "tiled/properties.h",
            "tiled/staggeredrenderer.cpp",
            "tiled/staggeredrenderer.h",
            "tiled/terrain.h",
            "tiled/tile.cpp",
            "tiled/tiled_global.h",
            "tiled/tiled.h",
            "tiled/tile.h",
            "tiled/tilelayer.cpp",
            "tiled/tilelayer.h",
            "tiled/tileset.cpp",
            "tiled/tileset.h",
        ]
        prefix: "src/"
    }

    Group {
        name: "enet code"
        files: [
            "callbacks.c",
            "compress.c",
            "host.c",
            "list.c",
            "packet.c",
            "peer.c",
            "protocol.c",
            "unix.c",
            "win32.c",
        ]
        prefix: "src/enet/"
        cpp.defines: [
            "HAS_GETHOSTBYADDR_R",
            "HAS_GETHOSTBYNAME_R",
            "HAS_POLL",
            "HAS_FCNTL",
            "HAS_INET_PTON",
            "HAS_INET_NTOP",
            "HAS_MSGHDR_FLAGS",
            "HAS_SOCKLEN_T",
        ]
    }

    cpp.includePaths: {
        var paths = ["src/"];
        paths.push("src/enet/include/");
        return paths;
    }

    cpp.dynamicLibraries: {
        var libraries = ["z"];
        return libraries;
    }

    cpp.defines: ["DEBUG_NETWORK"]
}
