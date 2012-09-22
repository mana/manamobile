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

#include "messageout.h"

#include <enet/enet.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

/** Initial amount of bytes allocated for the messageout data buffer. */
const unsigned int INITIAL_DATA_CAPACITY = 16;

/** Factor by which the messageout data buffer is increased when too small. */
const unsigned int CAPACITY_GROW_FACTOR = 2;

namespace Mana {

MessageOut::MessageOut():
    mPos(0)
{
    mData = (char*) malloc(INITIAL_DATA_CAPACITY);
    mDataSize = INITIAL_DATA_CAPACITY;
}

MessageOut::MessageOut(int id):
    mData(0),
    mPos(0),
    mDataSize(0),
    mDebugMode(false)
{
    bool debug = true;
    if (debug)
        id |= Mana::XXMSG_DEBUG_FLAG;

    writeInt16(id);
    mDebugMode = debug;
}

MessageOut::~MessageOut()
{
    free(mData);
}

void MessageOut::clear()
{
    mData = (char *) realloc(mData, INITIAL_DATA_CAPACITY);
    mDataSize = INITIAL_DATA_CAPACITY;
    mPos = 0;
}

void MessageOut::expand(size_t bytes)
{
    mData = (char*)realloc(mData, mPos + bytes);
    mDataSize = mPos + bytes;
}

void MessageOut::writeInt8(int value)
{
    if (mDebugMode)
        writeValueType(Mana::Int8);

    expand(mPos + 1);
    mData[mPos] = value;
    mPos += 1;
}

void MessageOut::writeInt16(int value)
{
    if (mDebugMode)
        writeValueType(Mana::Int16);

    expand(mPos + 2);
    uint16_t t = ENET_HOST_TO_NET_16(value);
    memcpy(mData + mPos, &t, 2);
    mPos += 2;
}

void MessageOut::writeInt32(int value)
{
    if (mDebugMode)
        writeValueType(Mana::Int32);

    expand(mPos + 4);
    uint32_t t = ENET_HOST_TO_NET_32(value);
    memcpy(mData + mPos, &t, 4);
    mPos += 4;
}

void MessageOut::writeString(const QByteArray &string, int length)
{
    if (mDebugMode)
    {
        writeValueType(Mana::String);
        writeInt16(length);
    }

    int stringLength = string.length();
    if (length < 0)
    {
        // Write the length at the start if not fixed
        writeInt16(stringLength);
        length = stringLength;
    }
    else if (length < stringLength)
    {
        // Make sure the length of the string is no longer than specified
        stringLength = length;
    }
    expand(mPos + length);

    // Write the actual string
    memcpy(mData + mPos, string.constData(), stringLength);

    if (length > stringLength)
    {
        // Pad remaining space with zeros
        memset(mData + mPos + stringLength, '\0', length - stringLength);
    }
    mPos += length;
}

std::ostream &operator <<(std::ostream &os, const MessageOut &msg)
{
    if (msg.length() >= 2)
    {
        unsigned short id = ENET_NET_TO_HOST_16(*(short*) msg.mData);
        os << std::setw(6) << std::hex << std::showbase << std::internal
           << std::setfill('0') << id
           << std::dec << " (" << msg.length() << " B)";
    }
    else
    {
        os << "Unknown"
           << std::dec << " (" << msg.length() << " B)";
    }
    return os;
}

QDebug operator <<(QDebug debug, const MessageOut &msg)
{
    std::stringstream ss;
    ss << msg;
    return debug << ss.str().c_str();
}

void MessageOut::writeValueType(Mana::ValueType type)
{
    expand(1);
    mData[mPos] = type;
    mPos += 1;
}

} // namespace Mana
