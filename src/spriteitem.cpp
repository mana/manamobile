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

#include "spriteitem.h"

#include <QPainter>

#include "resourcemanager.h"

#include "mana/resource/action.h"
#include "mana/resource/animation.h"
#include "mana/resource/imageset.h"

using namespace Mana;

SpriteItem::SpriteItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
    , mSpriteRef(0)
    , mDirection(Action::DIRECTION_DOWN)
    , mReady(false)
    , mAction(0)
    , mAnimation(0)
    , mFrameIndex(0)
    , mFrame(0)
{
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(timerTick()));
}

SpriteItem::~SpriteItem()
{
    mSprite->decRef();
}

void SpriteItem::setSpriteRef(const SpriteReference *sprite)
{
    if (sprite == mSpriteRef)
        return;

    if (sprite) {
        mSprite = ResourceManager::instance()->requestSpriteDefinition(
                    sprite->sprite, sprite->variant);

        if (mSprite->status() == Resource::Loading) {
            connect(mSprite, SIGNAL(statusChanged(Mana::Resource::Status)),
                    this, SLOT(statusChanged(Mana::Resource::Status)));
        } else {
            statusChanged(mSprite->status());
        }
    }

    setFlag(ItemHasNoContents, sprite == 0);

    emit spriteRefChanged();
}

void SpriteItem::reset()
{
    mFrameIndex = 0;
    mFrame = mAnimation->frame(0);
    updateTimer();
    update();
}

void SpriteItem::setAction(const QString &actionName)
{
    if (!mReady) {
        mActionName = actionName;
        return;
    }

    if (actionName == mActionName && mFrame)
        return;

    mActionName = actionName;

    const Action *action = mSprite->action(actionName);
    if (!action)
        return;

    playAnimation(action);

    updateSize();

    emit actionChanged();
}

void SpriteItem::playAnimation(const Action *action)
{
    mAction = action;
    const Animation *animation = mAction->animation(mDirection);

    if (animation && animation != mAnimation && animation->length() > 0) {
        mAnimation = animation;
        reset();
    }
}

void SpriteItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *,
                       QWidget *)
{
    if (!mFrame)
        return;

    painter->drawPixmap(QPoint(mFrame->offsetX, mFrame->offsetY),
                        *mFrame->imageset->pixmap(), mFrame->clip);
}

void SpriteItem::setDirection(Action::SpriteDirection direction)
{
    if (mDirection == direction)
        return;

    mDirection = direction;

    if (!mAction)
        return;

    playAnimation(mAction);

    emit directionChanged();
}

void SpriteItem::timerTick()
{
    mFrameIndex++;
    if (mFrameIndex == mAnimation->length())
        reset();
    else {
        mFrame = mAnimation->frame(mFrameIndex);
        updateTimer();

        if (Animation::isTerminator(*mFrame))
            setAction(SpriteAction::STAND);
        else
            update();
    }
}

void SpriteItem::statusChanged(Resource::Status status)
{
    if (status == Resource::Ready) {
        mReady = true;

        // start playing saved
        if (mActionName.size())
            setAction(mActionName);
    } else if (status == Resource::Error) {
        mSprite->decRef();
        mSprite = 0;
    }
}

void SpriteItem::updateTimer()
{
    mTimer.start(mFrame->delay);
}

void SpriteItem::updateSize()
{
    if (mFrame) {
        setImplicitWidth(mFrame->clip.width());
        setImplicitHeight(mFrame->clip.height());
    }
}
