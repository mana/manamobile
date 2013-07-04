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

#ifndef MESSAGEIN_H
#define MESSAGEIN_H

#include <iosfwd>

#include <QString>
#include <QDebug>

#include "protocol.h"

namespace Mana {

/**
 * Used for parsing an incoming message.
 */
class MessageIn
{
public:
    /**
     * Constructor.
     *
     * @param data   the message data
     * @param length the length of the data
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
     * Reads a byte array. If a length is not given (-1), it is assumed
     * that the length of the array is stored in a short at the
     * start of the array.
     */
    QByteArray readByteArray(int length = -1);

    /**
     * Returns the length of unread data.
     */
    int unreadLength() const { return mLength - mPos; }

private:
    bool readValueType(ValueType type);

    const char *mData;          /**< Packet data */
    int mLength;                /**< Length of data in bytes */
    unsigned short mId;         /**< The message ID. */
    bool mDebugMode;            /**< Includes debugging information. */

    /**
     * Actual position in the packet. From 0 to packet->length. A value
     * bigger than packet->length means EOP was reached when reading it.
     */
    int mPos;

    /**
     * Streams message ID and length to the given output stream.
     *
     * When the message includes debugging information, prints out
     * the message contents instead of the length.
     */
    friend std::ostream &operator <<(std::ostream &os, const MessageIn &msg);
    friend QDebug operator <<(QDebug debug, const MessageIn &msg);
};

} // namespace Mana

#endif // MESSAGEIN_H
