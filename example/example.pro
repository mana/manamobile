# Add more folders to ship with the application, here
folder_main.source = qml/main
folder_main.target = qml
DEPLOYMENTFOLDERS = folder_main

SOURCES += main.cpp
TARGET = tales

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
!tizen:qtcAddDeployment()

OTHER_FILES += \
    tizen/manifest.xml

RESOURCES += \
    main.qrc

tizen {
    TEMPLATE=app

    tizen_shared.files = shared/res
    CONFIG += TIZEN_STANDALONE_PACKAGE
    TIZEN_BUNDLED_QT_LIBS=Qt5Core Qt5DBus Qt5Qml Qt5Quick Qt5Widgets Qt5Gui Qt5Network

    tizen_data.files += $$_PRO_FILE_PWD_/qml
    tizen_data.files += $$_PRO_FILE_PWD_/../src/qml

    load(tizen_app)
}
