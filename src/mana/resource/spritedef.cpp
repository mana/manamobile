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

#include "spritedef.h"

#include "mana/resource/animation.h"
#include "mana/resource/imageresource.h"
#include "mana/resource/imageset.h"
#include "mana/resourcemanager.h"

#include "mana/xmlreader.h"

#include <QNetworkReply>

using namespace Mana;

SpriteReference::SpriteReference(QObject *parent, QString sprite, int variant)
    : QObject(parent)
    , sprite(sprite)
    , variant(variant)
{
}



SpriteReference *SpriteReference::readSprite(XmlReader &xml, QObject *parent)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "sprite");

    int variant = xml.attributes().value("variant").toInt();
    QString file = xml.readElementText();

    return new SpriteReference(parent, file, variant);
}

SpriteDefinition::SpriteDefinition(QObject *parent,
                                   const QUrl &url,
                                   int variant)
    : Resource(url, parent)
    , mVariant(variant)
    , mVariantCount(0)
    , mVariantOffset(0)
{
    setStatus(Resource::Loading);

    QString path = url.path(QUrl::FullyDecoded);
    int pos = path.indexOf(QLatin1Char('|'));
    if (pos != -1)
        mPalettes = path.mid(pos + 1);

    const QString pathWithoutDye = path.left(pos);
    QUrl urlWithoutDye = url;
    urlWithoutDye.setPath(pathWithoutDye, QUrl::DecodedMode);

    requestFile(urlWithoutDye);
}

SpriteDefinition::~SpriteDefinition()
{
}

const Action *SpriteDefinition::action(const QString &actionName) const
{
    QMap<QString, Action *>::const_iterator it = mActions.find(actionName);
    return it != mActions.end() ? it.value() : 0;
}

void SpriteDefinition::requestFile(const QUrl &url, XmlReader *parent)
{
    QSet<QUrl>::iterator it = mProcessedFiles.find(url);
    if (it != mProcessedFiles.end()) {
        qWarning() << "Cycle include of \"" << url << "\"!";
        cleanUp(Error);
    } else {
        mProcessedFiles.insert(url);
    }

    QNetworkReply *reply = ResourceManager::instance()->requestFile(url);
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

    readSprite(*xml, parent);
}

void SpriteDefinition::imageFileStatusChanged(Status newStatus)
{
    if (newStatus == Ready) {
        // continue parsing parent xml
        XmlReader *xml = mImageRequests[static_cast<ImageResource *>(sender())];
        readSprite(*xml, mResources[xml]);
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

void SpriteDefinition::readSprite(XmlReader &xml, XmlReader *parent)
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (!xml.isStartElement())
            continue;

        if (xml.name() == "sprite") {
            const QXmlStreamAttributes attr = xml.attributes();
            mVariantCount = attr.value("variants").toInt();
            mVariantOffset = attr.value("variant_offset").toInt();
        } else if (xml.name() == "include") {
            const QString filename = xml.attributes().value("file").toString();
            ResourceManager *resMan = ResourceManager::instance();
            QUrl url = resMan->resolve(resMan->spritePath() + filename);
            requestFile(url, &xml);
            return; // Wait for this file to be loaded first
        } else if (xml.name() == "action") {
            readAction(xml);
        } else if (xml.name() == "imageset") {
            if (readImageSet(xml))
                return; // Wait for it to be loaded first
        }
    }

    // This file is fully parsed. Continue parsing the parent
    if (parent)
        readSprite(*parent, mResources[parent]);
    else
        cleanUp(Ready);
}

void SpriteDefinition::readAction(XmlReader &xml)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "action");

    const QXmlStreamAttributes attr = xml.attributes();

    const QString actionName = attr.value("name").toString();
    const QString imageSetName = attr.value("imageset").toString();

    QMap<QString, ImageSet *>::iterator it = mImageSets.find(imageSetName);
    if (it == mImageSets.end()) {
        qWarning() << Q_FUNC_INFO << "Imageset"
                   << imageSetName << "not defined!";
        return;
    }
    ImageSet *imageSet = it.value();

    if (actionName == SpriteAction::INVALID) {
        qWarning() << "Unknown action " << actionName;
        return;
    }

    Action *action = new Action(this);
    mActions[actionName] = action;

    // When first action set it as default action
    if (mActions.size() == 1)
        mActions[SpriteAction::DEFAULT] = action;

    while (xml.readNextStartElement()) {
        if (xml.name() == "animation")
            readAnimation(xml, action, imageSet);
        else
            xml.readUnknownElement();
    }

}

static Action::SpriteDirection directionByName(const QStringRef &name)
{
    if (name.length() == 0 || name == "default")
        return Action::DIRECTION_DEFAULT;
    else if (name == "up")
        return Action::DIRECTION_UP;
    else if (name == "down")
        return Action::DIRECTION_DOWN;
    else if (name == "left")
        return Action::DIRECTION_LEFT;
    else if (name == "right")
        return Action::DIRECTION_RIGHT;
    else
        return Action::DIRECTION_INVALID;
}

void SpriteDefinition::readAnimation(XmlReader &xml,
                                     Action *action,
                                     ImageSet *imageSet)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "animation");

    const QXmlStreamAttributes attr = xml.attributes();

    const QStringRef directionName = attr.value("direction");
    const Action::SpriteDirection direction = directionByName(directionName);
    if (direction == Action::DIRECTION_INVALID) {
        qWarning() << Q_FUNC_INFO << "Unknown direction \""
                   << directionName << "\"";
        return;
    }

    Animation *animation = new Animation;
    action->setAnimation(direction, animation);

    while (xml.readNextStartElement()) {
        const QXmlStreamAttributes attr = xml.attributes();

        bool delayOk;
        int delay = attr.value("delay").toInt(&delayOk);
        int offsetX = attr.value("offsetX").toInt();
        int offsetY = attr.value("offsetY").toInt();

        if (!delayOk)
            delay = DEFAULT_FRAME_DELAY;
        else if (delay == 0)
            continue; // 0-delay frames never show

        offsetX += imageSet->offsetX();
        offsetY += imageSet->offsetY();

        if (xml.name() == "frame") {
            bool indexOk;
            const int index = attr.value("index").toInt(&indexOk);
            if (!indexOk || index < 0) {
                qWarning() << Q_FUNC_INFO << "No valid value for 'index'!";
                xml.skipCurrentElement();
                continue;
            }

            animation->addFrame(imageSet->imageResource(),
                                imageSet->clip(index + mVariantOffset),
                                delay, offsetX, offsetY);
            xml.skipCurrentElement();
        } else if (xml.name() == "sequence") {
            bool startOk, endOk;
            int start = attr.value("start").toInt(&startOk);
            const int end = attr.value("end").toInt(&endOk);

            if (!startOk || !endOk || start < 0 || end < 0) {
                qWarning() << Q_FUNC_INFO
                           << "No valid value for 'start' or 'end'";
                xml.skipCurrentElement();
                continue;
            }

            while (end >= start) {
                animation->addFrame(imageSet->imageResource(),
                                    imageSet->clip(start + mVariantOffset),
                                    delay, offsetX, offsetY);
                start++;
            }
            xml.skipCurrentElement();
        } else if (xml.name() == "end") {
            animation->addTerminator();
            xml.skipCurrentElement();
        } else {
            xml.readUnknownElement();
        }
    }
}

bool SpriteDefinition::readImageSet(XmlReader &xml)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "imageset");

    const QXmlStreamAttributes attr = xml.attributes();
    const QString name = attr.value("name").toString();

    if (name.isEmpty()) {
        qWarning() << "Empty name for imageset!";
        xml.skipCurrentElement();
        return false;
    }

    // Do not allow same imageset multiple times
    if (mImageSets.find(name) != mImageSets.end()) {
        qWarning() << "Duplicate use of image set name \"" << name << "\"!";
        xml.skipCurrentElement();
        return false;
    }

    const int width = attr.value("width").toInt();
    const int height = attr.value("height").toInt();
    const int offsetX = attr.value("offsetX").toInt();
    const int offsetY = attr.value("offsetY").toInt();
    QString imageSrc = attr.value("src").toString();

    // TODO: Dye

    ImageSet *imageSet =  new ImageSet(imageSrc, offsetX, offsetY,
                                       width, height, this);
    mImageSets[name] = imageSet;

    const Mana::ImageResource *imageResource = imageSet->imageResource();

    // Wait for the image to be ready, if necessary
    if (imageResource->status() == Resource::Loading) {
        connect(imageResource, SIGNAL(statusChanged(Resource::Status)),
                this, SLOT(imageFileStatusChanged(Resource::Status)));
        mImageRequests[imageResource] = &xml;
        return true;
    }

    return false;
}
