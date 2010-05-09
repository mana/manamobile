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

#ifndef MESSAGEIN_H
#define MESSAGEIN_H

#include <QByteArray>
#include <QDataStream>

/**
 * Makes it convenient to parse received messages.
 */
class MessageIn
{
public:
    inline MessageIn(const QByteArray &data)
        : mData(data)
        , mDataStream(mData)
        , mId(readInt16())
    {
    }

    inline qint16 id() const { return mId; }

    inline qint8 readInt8()
    {
        qint8 value;
        mDataStream >> value;
        return value;
    }

    inline qint16 readInt16()
    {
        qint16 value;
        mDataStream >> value;
        return value;
    }

    inline qint32 readInt32()
    {
        qint32 value;
        mDataStream >> value;
        return value;
    }

    inline QByteArray readString()
    {
        const qint16 length = readInt16();
        QByteArray string;
        string.resize(length);
        mDataStream.readRawData(string.data(), length);
        return string;
    }

private:
    QByteArray mData;
    QDataStream mDataStream;
    qint16 mId;
};

#endif // MESSAGEIN_H
