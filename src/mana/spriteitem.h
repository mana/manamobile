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

#ifndef SPRITEITEM_H
#define SPRITEITEM_H

#include "mana/resource/action.h"
#include "mana/resource/animation.h"
#include "mana/resource/spritedef.h"

#include <QQuickItem>
#include <QTimer>

namespace Mana {

class SpriteItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(const Mana::SpriteReference *spriteReference READ spriteRef WRITE setSpriteRef NOTIFY spriteRefChanged)
    Q_PROPERTY(QString action READ action WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(Mana::Action::SpriteDirection direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    explicit SpriteItem(QQuickItem *parent = 0);

    ~SpriteItem();

    void reset();

    void setSpriteRef(const SpriteReference *sprite);
    const SpriteReference *spriteRef() const { return mSpriteRef; }

    void setAction(const QString &actionName);
    QString action() const { return mActionName; }

    void setDirection(Action::SpriteDirection direction);
    Action::SpriteDirection direction() const { return mDirection; }

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *);

signals:
    void spriteRefChanged();
    void actionChanged();
    void directionChanged();

private slots:
    void timerTick();
    void statusChanged(Resource::Status status);

private:
    void playAnimation(const Action *action);
    void updateTimer();
    void updateSize();

    const SpriteReference *mSpriteRef;
    QString mActionName;
    Action::SpriteDirection mDirection;

    bool mReady;

    SpriteDefinition *mSprite;
    const Action *mAction;
    const Animation *mAnimation;
    int mFrameIndex;
    const Frame *mFrame;
    QTimer mTimer;
};

} // namespace Mana

#endif // SPRITEITEM_H
