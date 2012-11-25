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

#include "spritedef.h"

#include "action.h"
#include "animation.h"
#include "itemdb.h"
#include "imageset.h"
#include "resourcemanager.h"
#include "resource.h"
#include "spriteitem.h"

#include "mana/xmlreader.h"

#include <QNetworkReply>

using namespace Mana;

SpriteReference::SpriteReference(QObject *parent, QString sprite, int variant)
    : QObject(parent)
{
    this->sprite = sprite;
    this->variant = variant;
}



SpriteReference *SpriteReference::readSprite(XmlReader &xml, QObject *parent)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "sprite");

    int variant = xml.attributes().value("variant").toString().toInt();
    QString file = xml.readElementText();

    return new SpriteReference(parent, file, variant);
}

SpriteDefinition::SpriteDefinition(QObject *parent,
                                   const QString &filePath,
                                   int variant)
    : Resource(parent)
    , mVariant(variant)
    , mVariantCount(0)
    , mVariantOffset(0)
{
    setStatus(Resource::Loading);

    int pos = filePath.indexOf("|");
    if (pos != -1)
        mPalettes = filePath.right(filePath.length() - pos);

    mFilePathWithoutDye = filePath.left(pos);

    if (ItemDB::instance()->loaded()) {
        itemsFileFinished();
    } else {
        connect(ItemDB::instance(), SIGNAL(itemsChanged()),
                this, SLOT(itemsFileFinished()));
    }
}

SpriteDefinition::~SpriteDefinition()
{
    qDeleteAll(mImageSets);
    qDeleteAll(mActions);
}

void SpriteDefinition::itemsFileFinished()
{
    requestFile(mFilePathWithoutDye);
}

const Action *SpriteDefinition::action(const QString &actionName) const
{
    QMap<QString, Action *>::const_iterator it = mActions.find(actionName);
    return it != mActions.end() ? it.value() : 0;
}

void SpriteDefinition::requestFile(const QString &filePath, XmlReader *parent)
{
    QSet<QString>::iterator it = mProcessedFiles.find(filePath);
    if (it != mProcessedFiles.end()) {
        qWarning() << "Cycle include of file \"" << filePath << "\"!";
        cleanUp(Error);
    } else {
        mProcessedFiles.insert(filePath);
    }

    QNetworkReply *reply = ResourceManager::instance()->requestFile(filePath);
    connect(reply, SIGNAL(finished()), this, SLOT(xmlFileFinished()));
    mXmlRequests[reply] = parent;
}

void SpriteDefinition::xmlFileFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Failed to download sprite definition:"
                 << reply->url() << "\n"
                 << reply->errorString();
        cleanUp(Error);
        return;
    }

    // Find parent file and remove it from mRequests
    QMap<QNetworkReply *, XmlReader *>::iterator it = mXmlRequests.find(reply);
    Q_ASSERT(it != mXmlRequests.end());
    XmlReader *parent = it.value();
    it = mXmlRequests.erase(it);

    XmlReader *xml = new XmlReader(reply);

    mResources[xml] = parent;

    parseSprite(xml, parent);
}

void SpriteDefinition::imageFileStatusChanged(Status newStatus)
{
    if (newStatus == Ready) {
        // continue parsing parent xml
        XmlReader *xml = mImageRequests[static_cast<PixmapResource *>(sender())];
        parseSprite(xml, mResources[xml]);
    }
}

void SpriteDefinition::cleanUp(Status status)
{
    // Delete all xml readers, clear resource map.
    for (QMap<XmlReader *, XmlReader *>::iterator it = mResources.begin(),
         it_end = mResources.end(); it != it_end; ++it) {
        delete it.key();
    }
    mResources.clear();

    for (QMap<QNetworkReply *, XmlReader *>::iterator it = mXmlRequests.begin(),
         it_end = mXmlRequests.end(); it != it_end; ++it) {
        it.key()->deleteLater();
    }

    mXmlRequests.clear();

    mImageRequests.clear();

    setStatus(status);
}

void SpriteDefinition::parseSprite(XmlReader *xml, XmlReader *parent)
{
    while (!xml->atEnd()) {
        xml->readNext();
        if (!xml->isStartElement())
            continue;

        if (xml->name() == "sprite") {
            mVariantCount = xml->intAttribute("variants");
            mVariantOffset = xml->intAttribute("variant_offset");
        } else if (xml->name() == "include") {
            const QString filename = xml->attribute("file");
            requestFile("sprites/" + filename, xml);
            return; // Wait for this file to be loaded first
        } else if (xml->name() == "action") {
            loadAction(xml);
        } else if (xml->name() == "imageset") {
            if (loadImageSet(xml))
                return; // Wait for it to be loaded first2
        }
    }

    // This file is fully parsed. Continue parsing the parent
    if (parent)
        parseSprite(parent, mResources[parent]);
    else
        cleanUp(Ready);
}

void SpriteDefinition::loadAction(XmlReader *xml)
{
    const QString actionName = xml->attribute("name");
    const QString imageSetName = xml->attribute("imageset");
    QMap<QString, ImageSet *>::iterator it = mImageSets.find(imageSetName);
    if (it == mImageSets.end()) {
        qWarning() << Q_FUNC_INFO << "Imageset \""
                   << imageSetName << "\" not defined!";
        return;
    }
    ImageSet *imageSet = it.value();

    if (actionName == SpriteAction::INVALID) {
        qWarning() << "Unknown action " << actionName;
        return;
    }

    Action *action = new Action;
    mActions[actionName] = action;

    // When first action set it as default action
    if (mActions.size() == 1)
        mActions[SpriteAction::DEFAULT] = action;

    while (!(xml->isEndElement() && xml->name() == "action")) {
        xml->readNextStartElement();
        if (xml->name() == "animation")
            loadAnimation(xml, action, imageSet);
    }

}

static SpriteDirection directionByName(const QString &name)
{
    if (name.length() == 0 || name == "default")
        return DIRECTION_DEFAULT;
    else if (name == "up")
        return DIRECTION_UP;
    else if (name == "down")
        return DIRECTION_DOWN;
    else if (name == "left")
        return DIRECTION_LEFT;
    else if (name == "right")
        return DIRECTION_RIGHT;
    else
        return DIRECTION_INVALID;
}

void SpriteDefinition::loadAnimation(XmlReader *xml,
                                     Action *action,
                                     ImageSet *imageset)
{
    const QString directionName = xml->attribute("direction");
    const SpriteDirection direction = directionByName(directionName);
    if (direction == DIRECTION_INVALID) {
        qWarning() << Q_FUNC_INFO << "Unknown direction \""
                   << directionName << "\"";
        return;
    }

    Animation *animation = new Animation;
    action->setAnimation(direction, animation);

    while (!(xml->isEndElement() && xml->name() == "animation")) {
        if (!xml->isStartElement()) {
            xml->readNext();
            continue;
        }

        const int delay = xml->intAttribute("delay", DEFAULT_FRAME_DELAY);
        const int offsetX = xml->intAttribute("offsetX") + imageset->offsetX();
        const int offsetY = xml->intAttribute("offsetY") + imageset->offsetY();
        if (xml->name() == "frame") {
            const int index = xml->intAttribute("index", -1);
            if (index < 0) {
                qWarning() << Q_FUNC_INFO << "No valid value for 'index'!";
                xml->readNext();
                continue;
            }

            animation->addFrame(index + mVariantOffset, imageset,
                                delay, offsetX, offsetY);
        } else if (xml->name() == "sequence") {
            int start = xml->intAttribute("start", -1);
            const int end = xml->intAttribute("end", -1);

            if (start < 0 || end < 0) {
                qWarning() << Q_FUNC_INFO
                           << "No valid value for 'start' or 'end'";
                xml->readNext();
                continue;
            }

            while (end >= start) {
                animation->addFrame(start + mVariantOffset, imageset,
                                    delay, offsetX, offsetY);
                start++;
            }
        } else if (xml->name() == "end") {
            animation->addTerminator();
        }
        xml->readNext();
    }
}

bool SpriteDefinition::loadImageSet(XmlReader *xml)
{
    QString name = xml->attribute("name");

    if (name.isEmpty()) {
        qWarning() << "Empty name for imageset!";
        return false;
    }

    // Do not allow same imageset multiple times
    if (mImageSets.find(name) != mImageSets.end()) {
        qWarning() << "Duplicate use of image set name \"" << name << "\"!";
        return false;
    }

    const int width = xml->intAttribute("width");
    const int height = xml->intAttribute("height");
    const int offsetX = xml->intAttribute("offsetX");
    const int offsetY = xml->intAttribute("offsetY");
    QString imageSrc = xml->attribute("src");

    // TODO: Dye

    ImageSet *imageSet =  new ImageSet(imageSrc, offsetX, offsetY,
                                       width, height, this);
    connect(imageSet->pixmapResource(),
            SIGNAL(statusChanged(Mana::Resource::Status)),
            this, SLOT(imageFileStatusChanged(Mana::Resource::Status)));
    mImageRequests[imageSet->pixmapResource()] = xml;
    mImageSets[name] = imageSet;
    return true;
}
