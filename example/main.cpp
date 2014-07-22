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
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QScreen>

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_LINUX_TIZEN)
static QString adjustSharePath(const QString &path)
{
#if defined(Q_OS_MAC)
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#elif defined(Q_OS_QNX)
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("app/native/%1").arg(path);
#elif (defined(Q_OS_UNIX) || defined(Q_OS_WIN)) && !defined(Q_OS_ANDROID)
    const QString pathInInstallDir =
            QString::fromLatin1("%1/../share/tales-client/%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
    return path;
}
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("Source of Tales");
    app.setOrganizationDomain("sourceoftales.org");
    app.setOrganizationName(QLatin1String("tales"));
    app.setApplicationVersion("0.1");

    QFontDatabase::addApplicationFont("://fonts/DejaVuSerifCondensed.ttf");
    QFontDatabase::addApplicationFont("://fonts/DejaVuSerifCondensed-Italic.ttf");
    QFontDatabase::addApplicationFont("://fonts/DejaVuSerifCondensed-Bold.ttf");
    QFontDatabase::addApplicationFont("://fonts/DejaVuSerifCondensed-BoldItalic.ttf");
    app.setFont(QFont("DejaVu Serif"));

    QQmlApplicationEngine engine;

    QStringList arguments = app.arguments();

    QString customServerListPath;
    QString customServer = "server.sourceoftales.org";
    ushort customPort = 9601;
    bool fullScreen = false;

    for (int i = 1, max = arguments.length(); i < max; ++i) {
        if (arguments[i] == "--fullscreen") {
            fullScreen = true;
        } else if (arguments[i] == "--serverlist") {
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

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("customServerListPath", customServerListPath);
    context->setContextProperty("customServer", customServer);
    context->setContextProperty("customPort", customPort);

#ifdef Q_OS_ANDROID
    engine.addImportPath(QLatin1String("assets:/qml"));
    engine.addPluginPath(QDir::homePath() + "/../lib");
    engine.load(QUrl(QLatin1String("assets:/qml/main/mobile.qml")));
#elif defined(Q_OS_LINUX_TIZEN)
    engine.addImportPath(QLatin1String("../data/qml"));
    engine.load(app.applicationDirPath() +
                QLatin1String("/../data/qml/main/mobile.qml"));
#else
    engine.addImportPath(adjustSharePath(app.applicationDirPath() +
                                         QLatin1String("/../lib/libmana/qml/")));
    engine.load(adjustSharePath(QLatin1String("qml/main/mobile.qml")));
#endif

    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    if (!window) {
        qWarning() << "no window";
        return -1;
    }

    window->setClearBeforeRendering(false);

#ifdef Q_OS_LINUX_TIZEN
    window->setProperty("contentFollowsContentOrientation", true);
    window->screen()->setOrientationUpdateMask(Qt::LandscapeOrientation |
                                               Qt::InvertedLandscapeOrientation);
#endif

#if defined(Q_WS_SIMULATOR) || defined(Q_OS_QNX)
    window->showFullScreen();
#else
    if (fullScreen)
        window->showFullScreen();
    else
        window->show();
#endif

    return app.exec();
}

#ifdef Q_OS_LINUX_TIZEN
extern "C" int OspMain(int argc, char *argv[])
{
#ifdef Q_OS_LINUX_TIZEN_SIMULATOR
    qputenv("QSG_RENDER_LOOP", "windows");
#endif
    return main(argc, argv);
}
#endif
