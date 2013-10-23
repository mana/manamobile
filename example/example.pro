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
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/values-pl/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-el/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-id/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fr/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
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
