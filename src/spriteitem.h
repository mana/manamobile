/*
 * Copyright 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
 *
 * This file is part of Mana Mobile.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ANIMATEDSPRITEITEM_H
#define ANIMATEDSPRITEITEM_H

#include <QDeclarativeItem>
#include <QTimer>

#include "mana/resource/animation.h"
#include "mana/resource/spritedef.h"


class SpriteItem : public QDeclarativeItem
{
Q_OBJECT

public:
    explicit SpriteItem(Mana::SpriteReference *sprite,
                        QDeclarativeItem *parent = 0);

    ~SpriteItem();

    void reset();

    void play(const QString &actionName);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *,
               QWidget *);

    void setDirection(Mana::SpriteDirection direction);

private slots:
    void timerTick();
    void statusChanged(Mana::Resource::Status status);

private:
    void playAnimation(const Mana::Action *action);
    void updateTimer();

    bool mReady;

    Mana::SpriteDirection mDirection;
    Mana::SpriteDefinition *mSprite;
    const Mana::Action *mAction;
    QString mSavedAction;
    const Mana::Animation *mAnimation;
    int mFrameIndex;
    const Mana::Frame *mFrame;
    QTimer mTimer;


};

#endif // ANIMATEDSPRITEITEM_H
