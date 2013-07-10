/*
 * Mana QML plugin
 * Copyright (C) 2012  Erik Schilling 
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

#include "animation.h"

#include "imageset.h"

using namespace Mana;

Animation::Animation(QObject *parent)
    : QObject(parent)
    , mDuration(0)
{
}

void Animation::addFrame(int index,
                         ImageSet *imageset,
                         int delay,
                         int offsetX,
                         int offsetY)
{
    Frame frame = { imageset->clip(index), imageset, delay, offsetX, offsetY };
    mFrames.append(frame);
    mDuration += delay;
}

void Animation::addTerminator()
{
    Frame frame = { QRect(), 0, 0, 0, 0 };
    mFrames.append(frame);
}

bool Animation::isTerminator(const Frame &frame)
{
    return frame.imageset == 0;
}
