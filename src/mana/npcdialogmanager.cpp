/*
 * manalib
 * Copyright 2013, Erik Schilling <ablu.erikschilling@googlemail.com>
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

#include "npcdialogmanager.h"

#include "being.h"
#include "messagein.h"
#include "monster.h"
#include "npc.h"

using namespace Mana;

#ifndef QT_NO_DEBUG
#define ASSERT_ELSE(x) Q_ASSERT(x); if (!(x))
#else
#define ASSERT_ELSE(x) if (!(x))
#endif

NpcDialogManager::NpcDialogManager(QObject *parent) :
    QObject(parent),
    mExpectedInput(EXPECT_NOTHING),
    mNpc(0)
{
}

void NpcDialogManager::setNpc(Being *npc)
{
    if (npc != mNpc) {
        Q_ASSERT(npc->type() == Being::OBJECT_NPC);
        mNpc = static_cast<NPC *>(npc);

        emit npcChanged();
    }
}

void NpcDialogManager::startTalkingTo(Being *being)
{
    ASSERT_ELSE(being->type() == OBJECT_NPC)
        return;


    if (!mNpc) {
        mNpc = static_cast<NPC *>(being);
        emit npcChanged();
        emit startTalking(being->id());
    }
}

void NpcDialogManager::next()
{
    ASSERT_ELSE(mExpectedInput == EXPECT_NEXT && mNpc)
        return;

    emit nextMessage(mNpc->id());
}

void NpcDialogManager::choose(int choice)
{
    ASSERT_ELSE(mExpectedInput == EXPECT_CHOICE && mNpc)
        return;

    emit doChoice(mNpc->id(), choice);
}

void NpcDialogManager::handleNpcMessage(MessageIn &message)
{
    int id = message.readInt16();
    QString text = message.readString();

    mCurrentText = text;
    mExpectedInput = EXPECT_NEXT;
    emit currentTextChanged();
    emit expectedInputChanged();
}


void NpcDialogManager::handleNpcClose(MessageIn &message)
{
    int id = message.readInt16();

    mExpectedInput = EXPECT_NOTHING;
    mNpc = 0;
    emit expectedInputChanged();
    emit npcChanged();
}


void NpcDialogManager::handleNpcChoice(MessageIn &message)
{
    mCurrentChoices.clear();

    int id = message.readInt16();

    while (message.unreadLength())
        mCurrentChoices.append(message.readString());

    mExpectedInput = EXPECT_CHOICE;
    emit currentChoicesChanged();
    emit expectedInputChanged();
}
