/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "keys.h"

namespace gams {
namespace studio {

Keys *Keys::mInstance = nullptr;

Keys::Keys()
{

}

KeySeqList::KeySeqList(const QKeySequence& seq, QString title)
    : mTitle(title)
{
    mSequence << seq;
}

KeySeqList::KeySeqList(const char* seq, QString title)
    : mTitle(title)
{
    mSequence << QKeySequence(seq);
}

KeySeqList& KeySeqList::operator=(const KeySeqList& other)
{
    mSequence = other.mSequence;
    mTitle = other.mTitle;
    return *this;
}

KeySeqList&KeySeqList::operator=(const QKeySequence& other)
{
    mSequence.clear();
    mSequence << other;
    return *this;
}

KeySeqList&KeySeqList::operator <<(const KeySeqList& other)
{
    mSequence.append(other.mSequence);
    return *this;
}

KeySeqList&KeySeqList::operator <<(const QKeySequence& other)
{
    mSequence << other;
    return *this;
}

bool KeySeqList::operator ==(KeySeqList other) const
{
    bool res = mSequence.size() == other.mSequence.size();
    for (QKeySequence seq: mSequence) {
        if (!res) break;
        res = other.mSequence.contains(seq);
    }
    return res;
}

void Keys::reset()
{
    KeySeqList *seq;
    seq = new KeySeqList("Return","new line");
    *seq << QKeySequence("Enter");
    setHotkey(Hotkey::NewLine, seq);

    seq = new KeySeqList("Cut","cut selected text");
    *seq << QKeySequence("Ctrl+X");
    setHotkey(Hotkey::Cut, seq);

    seq = new KeySeqList("Copy","copy selected text");
    *seq << QKeySequence("Ctrl+C");
    setHotkey(Hotkey::Copy, seq);

    seq = new KeySeqList("Paste","paste clipboard content");
    *seq << QKeySequence("Ctrl+V");
    setHotkey(Hotkey::Paste, seq);

    seq = new KeySeqList("Undo","undo last modification");
    *seq << QKeySequence("Ctrl+Z");
    setHotkey(Hotkey::Undo, seq);

    seq = new KeySeqList("Redo","redo last modification");
    *seq << QKeySequence("Shift+Ctrl+Z");
    setHotkey(Hotkey::Redo, seq);

    seq = new KeySeqList("Shift+Alt+Up","start block edit");
    *seq << QKeySequence("Shift+Alt+Down") << QKeySequence("Shift+Alt+Left") << QKeySequence("Shift+Alt+Right");
    setHotkey(Hotkey::BlockEditStart, seq);

    seq = new KeySeqList("Esc","end block edit");
    *seq << QKeySequence("Up") << QKeySequence("Down") << QKeySequence("Left") << QKeySequence("Right")
         << QKeySequence("PgUp") << QKeySequence("PgDown") << QKeySequence("Home") << QKeySequence("End");
    setHotkey(Hotkey::BlockEditEnd, seq);

    seq = new KeySeqList("Shift+Ctrl+L","duplicate line");
    setHotkey(Hotkey::DuplicateLine, seq);

    seq = new KeySeqList("Ctrl+Shift+I","indent selected lines");
    *seq << QKeySequence("Tab");
    setHotkey(Hotkey::Indent, seq);

    seq = new KeySeqList("Ctrl+Shift+U","outdent selected lines");
    *seq << QKeySequence("Shift+Tab") << QKeySequence("Shift+Backtab");
    setHotkey(Hotkey::Outdent, seq);

    seq = new KeySeqList("Ctrl+Y","remove line");
    setHotkey(Hotkey::RemoveLine, seq);

    seq = new KeySeqList("F8","goto matching parenthesis");
    setHotkey(Hotkey::MatchParenthesis, seq);

    seq = new KeySeqList("Shift+F8","select to matching parenthesis");
    setHotkey(Hotkey::SelectParenthesis, seq);
}

void Keys::read(const QJsonObject& json)
{
    Q_UNUSED(json)
}

void Keys::write(QJsonObject& json) const
{
    Q_UNUSED(json)
}

void Keys::setHotkey(Hotkey key, KeySeqList* keySeqList)
{
    mHotkeyDefs.insert(key, keySeqList);
}


} // namespace studio
} // namespace gams
