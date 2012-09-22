/*
 *  manalib
 *  Copyright (C) 2004-2010  The Mana World Development Team
 *  Copyright (C) 2010  Thorbjørn Lindeijer
 *
 *  This file is part of manalib.
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

#include "messagein.h"

#include <enet/enet.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Not enabled by default since this will cause assertions on message errors,
// which may also originate from the client.
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
    mDebugMode = mId & Mana::XXMSG_DEBUG_FLAG;
    mId &= ~Mana::XXMSG_DEBUG_FLAG;
}

int MessageIn::readInt8()
{
    int value = 0;

    if (!readValueType(Mana::Int8))
        return value;

    if (mPos < mLength)
    {
        value = mData[mPos];
    }
    mPos++;
    return value;
}

int MessageIn::readInt16()
{
    int value = 0;

    if (!readValueType(Mana::Int16))
        return value;

    if (mPos + 2 <= mLength)
    {
        uint16_t t;
        memcpy(&t, mData + mPos, 2);
        value = (short) ENET_NET_TO_HOST_16(t);
    }
    mPos += 2;
    return value;
}

int MessageIn::readInt32()
{
    int value = 0;

    if (!readValueType(Mana::Int32))
        return value;

    if (mPos + 4 <= mLength)
    {
        uint32_t t;
        memcpy(&t, mData + mPos, 4);
        value = ENET_NET_TO_HOST_32(t);
    }
    mPos += 4;
    return value;
}

QString MessageIn::readString(int length)
{
    if (!readValueType(Mana::String))
        return QString();

    if (mDebugMode)
    {
        int fixedLength = readInt16();
        if (fixedLength != length)
        {
            // String does not have the expected length
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
    if (!readValueType(Mana::String))
        return QByteArray();

    if (mDebugMode)
    {
        int fixedLength = readInt16();
        if (fixedLength != length)
        {
            // String does not have the expected length
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

bool MessageIn::readValueType(Mana::ValueType type)
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

QDebug operator <<(QDebug debug, const MessageIn &msg)
{
    std::stringstream ss;
    ss << std::setw(6) << std::hex << std::showbase << std::internal
       << std::setfill('0') << msg.id() << std::dec;

    if (!msg.mDebugMode)
    {
        ss << " (" << msg.length() << " B)";
    }
    else
    {
        ss << " { ";

        MessageIn m(msg.mData, msg.mLength);

        while (m.unreadLength() > 0)
        {
            if (m.mPos > 3)
                ss << ", ";

            unsigned char valueType = m.mData[m.mPos];
            switch (valueType)
            {
            case Mana::Int8:
                ss << "B " << m.readInt8();
                break;
            case Mana::Int16:
                ss << "W " << m.readInt16();
                break;
            case Mana::Int32:
                ss << "D " << m.readInt32();
                break;
            case Mana::String: {
                // Peak ahead at a possible fixed length
                unsigned short pos = m.mPos;
                m.mPos++;
                int length = m.readInt16();
                m.mPos = pos;

                if (length < 0)
                    ss << "S " << m.readString().toStdString();
                else
                    ss << "S[" << length << "] " << m.readString(length).toStdString();
                break;
            }
            //case Mana::Double:
            //    ss << "d " << m.readDouble();
            //    break;
            default:
                ss << "??? }";
                return debug << ss.str().c_str();
            }
        }

        ss << " }";
    }
    return debug << ss.str().c_str();
}

} // namespace Mana
