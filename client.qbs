import qbs 1.0

CppApplication {
    name: "client"
    targetName: "tales-client"

    Depends {
        name: "Qt"
        submodules: [
            "quick",
            "gui",
        ]
    }

    Group {
        name: "Binaries"
        qbs.install: true
        qbs.installDir: "bin/"
        fileTagsFilter: "application"
    }

    Group {
        name: "C++ Files"
        prefix: "example/"
        files: ["main.cpp"]
    }

    Group {
        name: "Qt Resources"
        prefix: "example/"
        files: ["main.qrc"]
    }

    Group {
        name: "Client QML Files"
        prefix: "example/qml/main/"
        qbs.install: true
        qbs.installDir: "share/tales-client/qml/main/"
        files: [
            "ActionBar.qml",
            "AttributeLabel.qml",
            "BlueButton.qml",
            "BrownButton.qml",
            "Button.qml",
            "CharacterPage.qml",
            "ChatBar.qml",
            "ChatLog.qml",
            "Client.qml",
            "CompoundSprite.qml",
            "ConnectionStatusIndicator.qml",
            "ConnectionStatus.qml",
            "desktop.qml",
            "ErrorLabel.qml",
            "GamePage.qml",
            "HealthBar.qml",
            "ImageButton.qml",
            "InventoryPanel.qml",
            "Joystick.qml",
            "LineEdit.qml",
            "LoginPage.qml",
            "MainWindow.qml",
            "MessageDialog.qml",
            "mobile.qml",
            "NewCharacterPage.qml",
            "NpcDialog.qml",
            "OverheadChatMessage.qml",
            "PlayerAttributes.qml",
            "ProgressIndicator.qml",
            "QuestPanel.qml",
            "ResourcesWindow.qml",
            "RespawnButton.qml",
            "RightPanel.qml",
            "ScrollTitle.qml",
            "ServerListEntry.qml",
            "ServerPage.qml",
            "ShopWindow.qml",
            "StatusPage.qml",
            "TextShadow.qml",
            "Viewport.qml",
        ]
    }

    Group {
        name: "Images"
        qbs.install: true
        qbs.installDir: "share/tales-client/qml/main/images/"
        prefix: "example/qml/main/images/"
        files: [
            "bigbutton_disabled.png",
            "bigbutton_hovered.png",
            "bigbutton.png",
            "bigbutton_pressed.png",
            "bigroundbutton.png",
            "bigroundbutton_pressed.png",
            "bottombar.png",
            "bottom_frame.png",
            "button_disabled.png",
            "buttonhi.png",
            "button.png",
            "buttonpress.png",
            "chat_icon.png",
            "deepbox.png",
            "frame_button.png",
            "frame_button_pressed.png",
            "frame_with_title.png",
            "header_ornamental.png",
            "icon_money.png",
            "icon_right.png",
            "joystick_frame.png",
            "joystick.png",
            "lineedit.png",
            "login_background.png",
            "mana.svg",
            "progress-indicator.png",
            "roundbutton.png",
            "roundbutton_pressed.png",
            "scroll_button.png",
            "scroll_button_pressed.png",
            "scroll_footer.png",
            "scroll_header.png",
            "scroll_medium_horizontal.png",
            "scroll_thin.png",
            "simpleframe.png",
            "sourceoftales.png",
            "star.png",
            "tab_icon_character.png",
            "tab_icon_inventory.png",
            "tab_icon_questlog.png",
            "tab.png",
            "top_bar.png",
        ]
    }

}
