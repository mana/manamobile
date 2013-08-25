/*
 * Mana Mobile
 * Copyright (C) 2010  Thorbjørn Lindeijer
 * Copyright (C) 2012  Erik Schilling
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>

#include "qtquick2applicationviewer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("Mana Mobile");
    app.setOrganizationDomain("manasource.org");
    app.setOrganizationName(QLatin1String("mana"));
    app.setApplicationVersion("0.1");

    QtQuick2ApplicationViewer viewer;
    viewer.setTitle(app.applicationName());
    viewer.setClearBeforeRendering(false);

    QStringList arguments = app.arguments();

    QString customServerListPath;
    QString customServer;
    ushort customPort = 9601;

    for (int i = 1, max = arguments.length(); i < max; ++i) {
        if (arguments[i] == "--serverlist") {
            if (i + 1 < max)
                customServerListPath = arguments[++i];
            else
                qWarning() << "Missing argument for --serverlist";
        } else if (arguments[i] == "--server") {
            if (i + 1 < max)
                customServer = arguments[++i];
            else
                qWarning() << "Missing argument for --server";
        } else if (arguments[i] == "--port") {
            if (i + 1 < max)
                customPort = arguments[++i].toUShort();
            else
                qWarning() << "Missing argument for --port";
        }
    }

    QQmlContext* context = viewer.rootContext();
    context->setContextProperty("customServerListPath", customServerListPath);
    context->setContextProperty("customServer", customServer);
    context->setContextProperty("customPort", customPort);

#ifdef Q_OS_ANDROID
    viewer.engine()->addImportPath(QLatin1String("assets:/qml"));
    viewer.engine()->addPluginPath(QDir::homePath() + "/../lib");
#else
    viewer.engine()->addImportPath(app.applicationDirPath() +
                                   QLatin1String("/../src/qml/"));
#endif

    viewer.setMainQmlFile(QLatin1String("qml/main/mobile.qml"));

    viewer.showExpanded();

    return app.exec();
}
