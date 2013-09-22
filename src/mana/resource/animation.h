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

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QRectF>
#include <QVector>

namespace Mana {

class ImageResource;

struct Frame
{
    QRectF clip;
    qreal offsetX;
    qreal offsetY;
    const ImageResource *imageResource;
    int delay;
};

class Animation : public QObject
{
    Q_OBJECT

public:
    explicit Animation(QObject *parent = 0);

    void addFrame(const ImageResource *imageResource,
                  const QRectF &clip,
                  int delay,
                  qreal offsetX,
                  qreal offsetY);

    void addTerminator();

    const Frame *frame(int index) const { return &(mFrames[index]); }

    int length() const { return mFrames.size(); }
    int duration() const { return mDuration; }

    static bool isTerminator(const Frame &frame);

private:
    int mDuration;
    QVector<Frame> mFrames;
};

} // namespace Mana

#endif // ANIMATION_H
