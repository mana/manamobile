/*
 * Mana QML plugin
 * Copyright (C) 2004-2010  The Mana World Development Team
 * Copyright (C) 2010  Thorbjørn Lindeijer
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

#include "messagein.h"

#include <enet/enet.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Not enabled by default since this will cause assertions on message errors,
// which may also originate from the other side of the network.
//#define DEBUG_NETWORK

#ifdef DEBUG_NETWORK
#include <cassert>
#define ASSERT_IF(x) assert(x); if (x)
#else
#define ASSERT_IF(x) if (x)
#endif

namespace Mana {

MessageIn::MessageIn(const char *data, int length):
    mData(data),
    mLength(length),
    mDebugMode(false),
    mPos(0)
{
    // Read the message ID
    mId = readInt16();

    // Read and clear the debug flag
    mDebugMode = mId & XXMSG_DEBUG_FLAG;
    mId &= ~XXMSG_DEBUG_FLAG;
}

int MessageIn::readInt8()
{
    int value = -1;

    if (!readValueType(Int8))
        return value;

    ASSERT_IF (mPos < mLength)
    {
        value = mData[mPos];
    }
    else
    {
        qDebug() << "Unable to read 1 byte in " << mId << "!";
    }

    mPos += 1;
    return value;
}

int MessageIn::readInt16()
{
    int value = -1;

    if (!readValueType(Int16))
        return value;

    ASSERT_IF (mPos + 2 <= mLength)
    {
        quint16 t;
        memcpy(&t, mData + mPos, 2);
        value = (short) ENET_NET_TO_HOST_16(t);
    }
    else
    {
        qDebug() << "Unable to read 2 bytes in " << mId << "!";
    }

    mPos += 2;
    return value;
}

int MessageIn::readInt32()
{
    int value = -1;

    if (!readValueType(Int32))
        return value;

    ASSERT_IF (mPos + 4 <= mLength)
    {
        quint32 t;
        memcpy(&t, mData + mPos, 4);
        value = ENET_NET_TO_HOST_32(t);
    }
    else
    {
        qDebug() << "Unable to read 4 bytes in " << mId << "!";
    }

    mPos += 4;
    return value;
}

QString MessageIn::readString(int length)
{
    if (!readValueType(String))
        return QString();

    if (mDebugMode)
    {
        int fixedLength = readInt16();
#ifdef DEBUG_NETWORK
        assert(fixedLength == length);
#endif
        if (fixedLength != length)
        {
            qDebug() << "Expected string of length " << length
                     << " but received length " << fixedLength;
            mPos = mLength + 1;
            return QString();
        }
    }

    // Get string length
    if (length < 0)
    {
        length = readInt16();
    }

    // Make sure the string isn't erroneous
    if (length < 0 || mPos + length > mLength)
    {
        mPos = mLength + 1;
        return QString();
    }

    // Read the string
    const char *stringBeg = mData + mPos;
    const char *stringEnd = (const char *) memchr(stringBeg, '\0', length);
    const int stringLength = stringEnd ? stringEnd - stringBeg : length;
    const QString readString = QString::fromUtf8(stringBeg, stringLength);
    mPos += length;

    return readString;
}

QByteArray MessageIn::readByteArray(int length)
{
    if (!readValueType(String))
        return QByteArray();

    if (mDebugMode)
    {
        int fixedLength = readInt16();
#ifdef DEBUG_NETWORK
        assert(fixedLength == length);
#endif
        if (fixedLength != length)
        {
            qDebug() << "Expected string of length " << length
                     << " but received length " << fixedLength;
            mPos = mLength + 1;
            return QByteArray();
        }
    }

    // Get string length
    if (length < 0)
    {
        length = readInt16();
    }

    // Make sure the string isn't erroneous
    if (length < 0 || mPos + length > mLength)
    {
        mPos = mLength + 1;
        return QByteArray();
    }

    // Read the byte array
    const char *arrayBegin = mData + mPos;
    const QByteArray array(arrayBegin, length);
    mPos += length;

    return array;
}

bool MessageIn::readValueType(ValueType type)
{
    if (!mDebugMode) // Verification not possible
        return true;

    ASSERT_IF (mPos < mLength)
    {
        int t = (unsigned char) mData[mPos];
        ++mPos;

        ASSERT_IF (t == type)
        {
            return true;
        }
        else
        {
            qDebug() << "Attempt to read " << type << " but got " << t;
        }
    }
    else
    {
        qDebug() << "Attempt to read " << type << " but no more data available";
    }

    return false;
}

std::ostream &operator <<(std::ostream &os, const MessageIn &msg)
{
    os << std::setw(6) << std::hex << std::showbase << std::internal
       << std::setfill('0') << msg.id() << std::dec;

    if (!msg.mDebugMode)
    {
        os << " (" << msg.length() << " B)";
    }
    else
    {
        os << " { ";

        MessageIn m(msg.mData, msg.mLength);

        while (m.unreadLength() > 0)
        {
            if (m.mPos > 3)
                os << ", ";

            unsigned char valueType = m.mData[m.mPos];
            switch (valueType)
            {
            case Int8:
                os << "B " << m.readInt8();
                break;
            case Int16:
                os << "W " << m.readInt16();
                break;
            case Int32:
                os << "D " << m.readInt32();
                break;
            case String: {
                // Peak ahead at a possible fixed length
                unsigned short pos = m.mPos;
                m.mPos++;
                int length = m.readInt16();
                m.mPos = pos;

                if (length < 0)
                    os << "S " << m.readByteArray().constData();
                else
                    os << "S[" << length << "] " << m.readByteArray(length).constData();
                break;
            }
            //case Double:
            //    os << "d " << m.readDouble();
            //    break;
            default:
                os << "??? }";
                return os; // Stop after error
            }
        }

        os << " }";
    }

    return os;
}

QDebug operator <<(QDebug debug, const MessageIn &msg)
{
    std::stringstream ss;
    ss << msg;
    return debug << ss.str().c_str();
}

} // namespace Mana
