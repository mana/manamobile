/**
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Mana Mobile");
    a.setOrganizationDomain("manasource.org");

    MainWindow w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif
    return a.exec();
}
