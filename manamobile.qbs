import qbs 1.0
import qbs.probes as Probes

Project {
    CppApplication {
        name: "manamobile"

        Probes.IncludeProbe {
            id: enetIncludes
            names: "enet/enet.h"
        }

        Depends {
            name: "Qt";
            submodules: [
                "network",
                "xmlpatterns",
                "quick",
            ]
        }
        Group {
            name: "Binaries"
            qbs.install: true
            qbs.installDir: "bin"
            fileTagsFilter: "application"
        }

        Group {
            name: "Manamobile C++ code"
            files: [
                "durationlogger.h",
                "main.cpp",
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
                "mana/enetclient.cpp",
                "mana/enetclient.h",
                "mana/gameclient.cpp",
                "mana/gameclient.h",
                "mana/mapitem.cpp",
                "mana/mapitem.h",
                "mana/messagein.cpp",
                "mana/messagein.h",
                "mana/messageout.cpp",
                "mana/messageout.h",
                "mana/monster.cpp",
                "mana/monster.h",
                "mana/npc.cpp",
                "mana/npcdialogmanager.cpp",
                "mana/npcdialogmanager.h",
                "mana/npc.h",
                "mana/protocol.h",
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
            name: "QML files"
            qbs.install: true
            qbs.installDir: "bin/qml/main"
            files: ["*.qml"]
            prefix: "qml/main/"
        }
        Group {
            name: "Images"
            qbs.install: true
            qbs.installDir: "bin/qml/main/images"
            files: ["*.png", "*.svg"]
            prefix: "qml/main/images/"
        }

        Group {
            name: "QML application viewer"
            files: ["qtquick2applicationviewer.cpp", "qtquick2applicationviewer.h"]
            prefix: "qtquick2applicationviewer/"
        }

        Group {
            name: "enet code"
            condition: !enetIncludes.found
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
            var paths = ["src", "qtquick2applicationviewer"];
            if (!enetIncludes.found)
                paths.push("libs/enet/");
            return paths;
        }

        cpp.dynamicLibraries: {
            var libraries = ["z"];
            if (enetIncludes.found)
                libraries.push("enet")
            return libraries;
        }
    }
}
