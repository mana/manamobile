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

#ifndef NPCDIALOGMANAGER_H
#define NPCDIALOGMANAGER_H

#include <QObject>

#include <QStringList>

#include "messagein.h"
#include "npc.h"

namespace Mana {

class NpcDialogManager : public QObject
{
    Q_OBJECT

    Q_ENUMS(ExpectedInput)
    Q_PROPERTY(QString currentText READ currentText NOTIFY currentTextChanged)
    Q_PROPERTY(QStringList currentChoices READ currentChoices NOTIFY currentChoicesChanged)
    Q_PROPERTY(Mana::NPC *npc READ npc NOTIFY npcChanged)
    Q_PROPERTY(ExpectedInput expectedInput READ expectedInput NOTIFY expectedInputChanged)

public:
    enum ExpectedInput {
        EXPECT_NOTHING,
        EXPECT_NEXT,
        EXPECT_CHOICE
    };

    explicit NpcDialogManager(QObject *parent = 0);

    QString currentText() const { return mCurrentText; }
    QStringList currentChoices() const { return mCurrentChoices; }

    NPC *npc() const { return mNpc; }
    void setNpc(Being *npc);

    ExpectedInput expectedInput() const { return mExpectedInput; }

    Q_INVOKABLE void startTalkingTo(Mana::Being *being);
    Q_INVOKABLE void next();
    Q_INVOKABLE void choose(int choice);

    void handleNpcMessage(MessageIn &message);
    void handleNpcClose(MessageIn &message);
    void handleNpcChoice(MessageIn &message);

signals:
    void startTalking(int id);
    void nextMessage(int id);
    void doChoice(int npcId, int choice);

    void currentTextChanged();
    void currentChoicesChanged();
    void npcChanged();
    void expectedInputChanged();

private:
    QString mCurrentText;

    QStringList mCurrentChoices;

    ExpectedInput mExpectedInput;

    NPC *mNpc;

};

}

#endif // NPCDIALOGMANAGER_H
