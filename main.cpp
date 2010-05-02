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
