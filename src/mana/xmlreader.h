/*
 * Mana QML plugin
 * Copyright (C) 2008-2013  Thorbjørn Lindeijer 
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <QtDebug>
#include <QXmlStreamReader>

#ifndef XMLREADER_H
#define XMLREADER_H

class XmlReader : public QXmlStreamReader
{
public:
    XmlReader(QIODevice *device) : QXmlStreamReader(device) {}

    void readUnknownElement()
    {
        qDebug() << "Unknown element at line" << lineNumber()
                 << "(fixme):" << name();
        skipCurrentElement();
    }
};

#endif // XMLREADER_H
