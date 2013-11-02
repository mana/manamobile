/*
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

#include "spriteitem.h"

#include "mana/resourcemanager.h"

#include "mana/resource/action.h"
#include "mana/resource/animation.h"
#include "mana/resource/imageresource.h"

#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGTextureMaterial>

using namespace Mana;

namespace {

class SubRectTextureNode : public QSGGeometryNode
{
public:
    SubRectTextureNode();

    void setTexture(QSGTexture *texture);
    void setRects(const QRectF &rect, const QRectF &sourceRect);

private:
    void update();

    QSGGeometry mGeometry;
    QSGTextureMaterial mMaterial;
    QSGOpaqueTextureMaterial mOpaqueMaterial;
    QRectF mRect;
    QRectF mSourceRect;
};

SubRectTextureNode::SubRectTextureNode()
    : mGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
{
    setGeometry(&mGeometry);
    setMaterial(&mMaterial);
    setOpaqueMaterial(&mOpaqueMaterial);
}

void SubRectTextureNode::setTexture(QSGTexture *texture)
{
    if (mMaterial.texture() == texture)
        return;

    mMaterial.setTexture(texture);
    mOpaqueMaterial.setTexture(texture);
    update();
    markDirty(DirtyMaterial);
}

void SubRectTextureNode::setRects(const QRectF &rect, const QRectF &sourceRect)
{
    if (mRect == rect && mSourceRect == sourceRect)
        return;

    mRect = rect;
    mSourceRect = sourceRect;
    update();
    markDirty(DirtyGeometry);
}

void SubRectTextureNode::update()
{
    QSGTexture *texture = mMaterial.texture();
    QRectF sourceRect(texture->convertToNormalizedSourceRect(mSourceRect));
    QSGGeometry::updateTexturedRectGeometry(&mGeometry, mRect, sourceRect);
}

} // anonymous namespace


SpriteItem::SpriteItem(QQuickItem *parent)
    : QQuickItem(parent)
    , mSpriteRef(0)
    , mDirection(Action::DIRECTION_DOWN)
    , mReady(false)
    , mSprite(0)
    , mAction(0)
    , mAnimation(0)
    , mFrameIndex(0)
    , mFrame(0)
    , mDisplayedFrame(0)
    , mUnusedTime(0)
    , mRunning(false)
{
    setFlag(ItemHasContents);
}

SpriteItem::~SpriteItem()
{
    if (mSprite)
        mSprite->decRef();
}

void SpriteItem::setSpriteRef(SpriteReference *sprite)
{
    if (sprite == mSpriteRef)
        return;

    if (mSprite) {
        mSprite->decRef();
        mSprite = 0;
    }

    if (sprite) {
        mSprite = ResourceManager::instance()->requestSpriteDefinition(
                    sprite->sprite(), sprite->variant());

        if (mSprite->status() == Resource::Loading) {
            connect(mSprite, SIGNAL(statusChanged(Resource::Status)),
                    this, SLOT(statusChanged(Resource::Status)));
        } else {
            statusChanged(mSprite->status());
        }
    }

    emit spriteRefChanged();
}

void SpriteItem::reset()
{
    mFrameIndex = 0;
    mFrame = mAnimation->frame(0);
    mRunning = mAnimation->length() > 1;

    if (mRunning) {
        mTimer.start();
        mUnusedTime = 0;
    }

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

QSGNode *SpriteItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    SubRectTextureNode *n = static_cast<SubRectTextureNode *>(node);

    if (mRunning)
        advance();

    if (mFrame != mDisplayedFrame) {
        mDisplayedFrame = mFrame;

        if (!mFrame) {
            delete n;
            return 0;
        }

        if (!n) {
            n = new SubRectTextureNode;
            n->setFlag(QSGNode::OwnedByParent);
        }

        QSGTexture *texture = mFrame->imageResource->texture(window());
        QRectF rect(mFrame->offsetX,
                    mFrame->offsetY,
                    mFrame->clip.width(),
                    mFrame->clip.height());

        n->setTexture(texture);
        n->setRects(rect, mFrame->clip);
    }

    if (mRunning)
        update();

    return n;
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

void SpriteItem::playAction(const QString &actionName)
{
    setAction(actionName);
    if (mAction)
        playAnimation(mAction);
}

void SpriteItem::advance()
{
    // TODO: Since every sprite item is keeping its own QElapsedTimer, there
    // is a small chance that the frames of the sprites in a CompoundSprite
    // will not match up.
    mUnusedTime += mTimer.restart();

    while (mFrame->delay > 0 && mUnusedTime > mFrame->delay) {
        mUnusedTime -= mFrame->delay;
        mFrameIndex = (mFrameIndex + 1) % mAnimation->length();
        mFrame = mAnimation->frame(mFrameIndex);

        // Automatically go back to standing after finishing an animation
        if (Animation::isTerminator(*mFrame)) {
            setAction(SpriteAction::STAND);
            if (!mRunning)
                break;
        }
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

void SpriteItem::updateSize()
{
    if (mFrame) {
        setImplicitSize(mFrame->clip.width(),
                        mFrame->clip.height());
    }
}
