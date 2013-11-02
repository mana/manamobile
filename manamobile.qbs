import qbs 1.0

Project {
    references: ["libmana.qbs"]

    CppApplication {
        name: "manamobile"

        Depends {
            name: "Qt";
            submodules: [
                "network",
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
                "main.cpp",
            ]
            prefix: "example/"
        }

        Group {
            name: "QML files"
            qbs.install: true
            qbs.installDir: "bin/qml/main"
            files: ["*.qml"]
            prefix: "example/qml/main/"
        }
        Group {
            name: "Images"
            qbs.install: true
            qbs.installDir: "bin/qml/main/images"
            files: ["*.png", "*.svg"]
            prefix: "example/qml/main/images/"
        }

        Group {
            name: "QML application viewer"
            files: ["qtquick2applicationviewer.cpp", "qtquick2applicationviewer.h"]
            prefix: "example/qtquick2applicationviewer/"
        }

        cpp.includePaths: ["example/qtquick2applicationviewer/"]
    }
}
