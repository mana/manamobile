/*
 *  Mana Mobile
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *
 *  This file is part of Mana Mobile.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QMessageBox>
#include <QDeclarativeContext>
#include <qdeclarative.h>

#ifndef QT_NO_OPENGL
#include <QGLWidget>
#endif

#include <enet/enet.h>

#include "qmlapplicationviewer.h"

#include "characterlistmodel.h"
#include "loginmanager.h"
#include "root.h"

static void registerTypes()
{
    qmlRegisterUncreatableType<LoginManager>("Mana", 1, 0, "LoginManager",
                                             "Use global loginManager");
    qmlRegisterUncreatableType<CharacterListModel>(
                "Mana", 1, 0, "CharacterListModel",
                "Use loginManager.characterListModel");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Mana Mobile");
    app.setOrganizationDomain("manasource.org");
    app.setApplicationVersion("0.1");

    if (enet_initialize() != 0)
    {
        QMessageBox::critical(0, app.applicationName(),
                              "An error occurred while initializing ENet.\n");
        return 1;
    }
    atexit(enet_deinitialize);

    registerTypes();

    QmlApplicationViewer viewer;
#ifndef QT_NO_OPENGL
    viewer.setViewport(new QGLWidget);
#endif
    viewer.setOrientation(QmlApplicationViewer::Auto);
    viewer.setWindowTitle(app.applicationName());

    Root *root = new Root(&viewer);
    viewer.rootContext()->setContextObject(root);

    viewer.setMainQmlFile(QLatin1String("qml/main/main.qml"));
    viewer.show();

    return app.exec();
}
