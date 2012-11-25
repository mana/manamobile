/*
 * manalib
 * Copyright 2011, Jared Adams <jaxad0127@gmail.com>
 * Copyright 2012, Erik Schilling <ablu.erikschilling@googlemail.com>
 *
 * This file is part of manalib.
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

#ifndef SPRITEDEF_H
#define SPRITEDEF_H

#include <QList>
#include <QMap>
#include <QPixmap>
#include <QSet>
#include <QString>

#include "resource.h"
#include "mana/being.h"
#include "mana/protocol.h"

class QNetworkReply;
class XmlReader;

namespace Mana {

class Action;
class ImageSet;
class PixmapResource;

class SpriteReference : public QObject
{
    Q_OBJECT

public:
    static SpriteReference *readSprite(XmlReader &xml, QObject *parent);

    SpriteReference(QObject *parent, QString sprite = "", int variant = 0);

    QString sprite;
    int variant;
};

struct SpriteDisplay
{
    QString image;
    QList<SpriteReference*> sprites;
    QList<QString> particles;
};

static int DEFAULT_FRAME_DELAY = 75;

namespace SpriteAction {
static const QString DEFAULT        = "stand";
static const QString STAND          = "stand";
static const QString SIT            = "sit";
static const QString SLEEP          = "sleep";
static const QString DEAD           = "dead";
static const QString WALK           = "walk";
static const QString ATTACK         = "attack";
static const QString HURT           = "hurt";
static const QString USE_SPECIAL    = "special";
static const QString CAST_MAGIC     = "magic";
static const QString USE_ITEM       = "item";
static const QString INVALID        = "";

static inline const QString &actionByInt(int integer)
{
    switch (integer) { // TODO (in this commit): fix silly values here and in protocol
    case Mana::STAND:
        return DEFAULT;
    case Mana::SIT:
        return SIT;
    case Mana::DEAD:
        return DEAD;
    case Mana::WALK:
        return WALK;
    case Mana::ATTACK:
        return ATTACK;
    case Mana::HURT:
        return HURT;
    default:
        return INVALID;
    }
}

}

enum SpriteDirection
{
    DIRECTION_DEFAULT = 0,
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_INVALID
};

static inline SpriteDirection spriteDirectionByBeing(BeingDirection dir)
{
    SpriteDirection spriteDirection = DIRECTION_DEFAULT;
    if (dir == UP)
        return DIRECTION_UP;
    else if (dir == DOWN)
        return DIRECTION_DOWN;
    else if (dir == LEFT)
        return DIRECTION_LEFT;
    else if (dir == RIGHT)
        return DIRECTION_RIGHT;

    return DIRECTION_DEFAULT;
}

class SpriteDefinition : public Resource
{
Q_OBJECT

public:
    SpriteDefinition(QObject *parent, const QString &filePath, int variant);
    virtual ~SpriteDefinition();

    const Action *action(const QString &actionName) const;

private slots:
    void itemsFileFinished();
    void xmlFileFinished();
    void imageFileStatusChanged(Mana::Resource::Status newStatus);

private:
    void requestFile(const QString &filePath, XmlReader *parent = 0);
    void cleanUp(Status status);
    void parseSprite(XmlReader *xml, XmlReader *parent);
    void loadAction(XmlReader *xml);
    void loadAnimation(XmlReader *xml, Action *action, ImageSet *imageset);
    bool loadImageSet(XmlReader *xml);

    int mVariant;
    int mVariantCount;
    int mVariantOffset;

    // Dye string
    QString mPalettes;

    QString mFilePathWithoutDye;

    // xml reader -> his reader parent (for includes)
    // parent parsing has to be resumed after child is fully parsed
    QMap<XmlReader *, XmlReader *> mResources;

    // Maps replies to their parent XmlReader
    QMap<QNetworkReply *, XmlReader *> mXmlRequests;

    // Maps replies to their parent XmlReader
    QMap<const PixmapResource *, XmlReader *> mImageRequests;

    QMap<QString, ImageSet *> mImageSets;

    QMap<QString, Action *> mActions;

    // Keep track of loaded files to prevent cycle includes
    QSet<QString> mProcessedFiles;
};

} // namespace Mana

#endif // SPRITEDEF_H
