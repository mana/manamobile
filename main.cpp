/**
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#include "mainwindow.h"

#include <enet/enet.h>

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Mana Mobile");
    a.setOrganizationDomain("manasource.org");

    if (enet_initialize() != 0)
    {
        QMessageBox::critical(0, a.applicationName(),
                              "An error occurred while initializing ENet.\n");
        return 1;
    }
    atexit(enet_deinitialize);

    MainWindow w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif
    return a.exec();
}
