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

#ifndef MESSAGEOUT_H
#define MESSAGEOUT_H

#include <iosfwd>

namespace Mana {

/**
 * Used for building an outgoing message.
 */
class MessageOut
{
public:
    /**
     * Constructor.
     */
    MessageOut();

    /**
     * Constructor that takes a message ID.
     */
    MessageOut(int id);

    /**
     * Destructor.
     */
    ~MessageOut();

    /**
     * Clears current message.
     */
    void clear();

    void writeInt8(int value);     /**< Writes an integer on one byte. */
    void writeInt16(int value);    /**< Writes an integer on two bytes. */
    void writeInt32(int value);    /**< Writes an integer on four bytes. */

    /**
     * Writes a 3-byte block containing tile-based coordinates.
     */
    void writeCoordinates(int x, int y);

    /**
     * Writes a string. If a fixed length is not given (-1), it is stored
     * as a short at the start of the string.
     */
    void writeString(const std::string &string, int length = -1);

    /**
     * Returns the content of the message.
     */
    char *data() const { return mData; }

    /**
     * Returns the length of the data.
     */
    unsigned int length() const { return mPos; }

private:
    /**
     * Ensures the capacity of the data buffer is large enough to hold the
     * given amount of bytes.
     */
    void expand(size_t size);

    char *mData;                         /**< Data building up. */
    unsigned int mPos;                   /**< Position in the data. */
    unsigned int mDataSize;              /**< Allocated datasize. */

    /**
     * Streams message ID and length to the given output stream.
     */
    friend std::ostream& operator <<(std::ostream &os,
                                     const MessageOut &msg);
};

} // namespace Mana

#endif // MESSAGEOUT_H
