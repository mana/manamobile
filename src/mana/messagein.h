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

#ifndef MESSAGEIN_H
#define MESSAGEIN_H

#include <iosfwd>

#include <QString>
#include <QDebug>

namespace Mana {

/**
 * Used for parsing an incoming message.
 */
class MessageIn
{
public:
    /**
     * Constructor.
     */
    MessageIn(const char *data, int length);

    /**
     * Returns the message ID.
     */
    unsigned short id() const { return mId; }

    /**
     * Returns the total length of this message.
     */
    int length() const { return mLength; }

    int readInt8();             /**< Reads an 8-bit integer. */
    int readInt16();            /**< Reads a 16-bit integer. */
    int readInt32();            /**< Reads a 32-bit integer. */

    /**
     * Reads a string. If a length is not given (-1), it is assumed
     * that the length of the string is stored in a short at the
     * start of the string.
     */
    QString readString(int length = -1);

    /**
     * Returns the length of unread data.
     */
    int unreadLength() const { return mLength - mPos; }

private:
    const char *mData;          /**< Packet data */
    int mLength;                /**< Length of data in bytes */
    unsigned short mId;         /**< The message ID. */

    /**
     * Actual position in the packet. From 0 to packet->length. A value
     * bigger than packet->length means EOP was reached when reading it.
     */
    int mPos;

    /**
     * Streams message ID and length to the given output stream.
     */
    friend std::ostream &operator <<(std::ostream &os, const MessageIn &msg);
    friend QDebug operator <<(QDebug debug, const MessageIn &msg);
};

} // namespace Mana

#endif // MESSAGEIN_H
