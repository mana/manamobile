/*
 * Mana QML plugin
 * Copyright (C) 2011  Jared Adams 
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

#ifndef SPRITEDEF_H
#define SPRITEDEF_H

#include <QMap>
#include <QSet>
#include <QStringList>

#include "action.h"
#include "resource.h"

#include "mana/being.h"
#include "mana/protocol.h"

class QNetworkReply;
class XmlReader;

namespace Mana {

class ImageSet;
class ImageResource;

class SpriteReference : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString sprite READ sprite WRITE setSprite NOTIFY spriteChanged)
    Q_PROPERTY(int variant READ variant WRITE setVariant NOTIFY variantChanged)

public:
    static SpriteReference *readSprite(XmlReader &xml, QObject *parent = 0);

    explicit SpriteReference(QObject *parent = 0);
    SpriteReference(QObject *parent, QString sprite = QString(), int variant = 0);

    void setSprite(const QString &spritePath);
    const QString &sprite() const;

    void setVariant(int variant);
    int variant() const;

signals:
    void spriteChanged();
    void variantChanged();

private:
    QString mSprite;
    int mVariant;
};

struct SpriteDisplay
{
    QString image;
    QList<SpriteReference*> sprites;
    QStringList particles;
};

static const int DEFAULT_FRAME_DELAY = 75;

namespace SpriteAction {

static const QString DEFAULT        = "stand";
static const QString STAND          = "stand";
static const QString SIT            = "sit";
static const QString SLEEP          = "sleep";
static const QString DEAD           = "dead";
static const QString WALK           = "walk";
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
    case Mana::HURT:
        return HURT;
    default:
        return INVALID;
    }
}

}

static inline Action::SpriteDirection beingToSpriteDirection(BeingDirection dir)
{
    if (dir == UP)
        return Action::DIRECTION_UP;
    else if (dir == DOWN)
        return Action::DIRECTION_DOWN;
    else if (dir == LEFT)
        return Action::DIRECTION_LEFT;
    else if (dir == RIGHT)
        return Action::DIRECTION_RIGHT;

    return Action::DIRECTION_DEFAULT;
}

class SpriteDefinition : public Resource
{
    Q_OBJECT

public:
    SpriteDefinition(QObject *parent, const QUrl &url, int variant);
    virtual ~SpriteDefinition();

    const Action *action(const QString &actionName) const;

private slots:
    void xmlFileFinished();
    void imageFileStatusChanged(Resource::Status newStatus);

private:
    void requestFile(const QUrl &url, XmlReader *parent = 0);
    void cleanUp(Status status);
    void readSprite(XmlReader &xml, XmlReader *parent);
    void readAction(XmlReader &xml);
    void readAnimation(XmlReader &xml, Action *action, ImageSet *imageset);
    bool readImageSet(XmlReader &xml);

    int mVariant;
    int mVariantCount;
    int mVariantOffset;

    // Dye string
    QString mPalettes;

    // xml reader -> his reader parent (for includes)
    // parent parsing has to be resumed after child is fully parsed
    QMap<XmlReader *, XmlReader *> mResources;

    // Maps replies to their parent XmlReader
    QMap<QNetworkReply *, XmlReader *> mXmlRequests;

    // Maps replies to their parent XmlReader
    QMap<const ImageResource *, XmlReader *> mImageRequests;

    QMap<QString, ImageSet *> mImageSets;

    QMap<QString, Action *> mActions;

    // Keep track of loaded files to prevent cycle includes
    QSet<QUrl> mProcessedFiles;
};


inline void SpriteReference::setSprite(const QString &spritePath)
{
    if (mSprite != spritePath) {
        mSprite = spritePath;
        emit spriteChanged();
    }
}

inline const QString &SpriteReference::sprite() const
{
    return mSprite;
}

inline void SpriteReference::setVariant(int variant)
{
    if (mVariant != variant) {
        mVariant = variant;
        emit variantChanged();
    }
}

inline int SpriteReference::variant() const
{
    return mVariant;
}

} // namespace Mana

Q_DECLARE_METATYPE(const Mana::SpriteReference*)

#endif // SPRITEDEF_H
