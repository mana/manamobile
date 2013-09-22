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

using namespace Mana;

Animation::Animation(QObject *parent)
    : QObject(parent)
    , mDuration(0)
{
}

/**
 * Adds a frame to the animation. The delay should be either positive or -1
 * (in which case the frame will show forever).
 *
 * @param imageResource The image that includes the frame.
 * @param clip          The bounding rectangle of the frame on the image.
 * @param delay         The delay of the frame in ms.
 * @param offsetX
 * @param offsetY
 */
void Animation::addFrame(const ImageResource *imageResource,
                         const QRectF &clip,
                         int delay,
                         qreal offsetX,
                         qreal offsetY)
{
    Q_ASSERT(delay != 0);

    Frame frame = { clip, offsetX, offsetY, imageResource, delay };
    mFrames.append(frame);
    mDuration += delay;
}

void Animation::addTerminator()
{
    Frame frame = { QRectF(), 0, 0, 0, 0 };
    mFrames.append(frame);
}

bool Animation::isTerminator(const Frame &frame)
{
    return frame.imageResource == 0;
}
