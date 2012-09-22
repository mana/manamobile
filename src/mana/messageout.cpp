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
    mPos(0)
{
    mData = (char*) malloc(INITIAL_DATA_CAPACITY);
    mDataSize = INITIAL_DATA_CAPACITY;

    writeInt16(id);
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
    if (bytes > mDataSize)
    {
        do
        {
            mDataSize *= CAPACITY_GROW_FACTOR;
        }
        while (bytes > mDataSize);

        mData = (char*) realloc(mData, mDataSize);
    }
}

void MessageOut::writeInt8(int value)
{
    expand(mPos + 1);
    mData[mPos] = value;
    mPos += 1;
}

void MessageOut::writeInt16(int value)
{
    expand(mPos + 2);
    quint16 t = ENET_HOST_TO_NET_16(value);
    memcpy(mData + mPos, &t, 2);
    mPos += 2;
}

void MessageOut::writeInt32(int value)
{
    expand(mPos + 4);
    quint32 t = ENET_HOST_TO_NET_32(value);
    memcpy(mData + mPos, &t, 4);
    mPos += 4;
}

void MessageOut::writeCoordinates(int x, int y)
{
    expand(mPos + 3);
    char *p = mData + mPos;
    p[0] = x & 0x00FF;
    p[1] = ((x & 0x0700) >> 8) | ((y & 0x001F) << 3);
    p[2] = (y & 0x07E0) >> 5;
    mPos += 3;
}

void MessageOut::writeString(const QByteArray &string, int length)
{
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

} // namespace Mana
