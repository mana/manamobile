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

#ifndef MESSAGEOUT_H
#define MESSAGEOUT_H

#include <QByteArray>
#include <QDataStream>

/**
 * Makes it convenient to send outgoing messages.
 */
class MessageOut
{
public:
    inline MessageOut(qint16 id)
        : mDataStream(&mData, QIODevice::WriteOnly)
    {
        writeInt16(id);
    }

    inline void writeInt8(qint8 value)
    { mDataStream << value; }

    inline void writeInt16(qint16 value)
    { mDataStream << value; }

    inline void writeInt32(qint32 value)
    { mDataStream << value; }

    inline void writeString(const QByteArray &string, qint16 length = -1)
    {
        // TODO: Support length == -1? (relevant for manaserv?)
        writeInt16(string.length());
        mDataStream.writeRawData(string.constData(), string.length());
    }

    inline const QByteArray &data() const { return mData; }

private:
    QByteArray mData;
    QDataStream mDataStream;
};

#endif // MESSAGEOUT_H
