/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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
#include "filecontext.h"
#include "filegroupcontext.h"
#include "exception.h"
#include "codeeditor.h"
#include "logger.h"

namespace gams {
namespace studio {

const QStringList FileContext::mDefaulsCodecs = QStringList() << "Utf-8" << "GB2312" << "Shift-JIS"
                                                              << "System" << "Windows-1250" << "Latin-1";

FileContext::FileContext(int id, QString name, QString location, ContextType type)
    : FileSystemContext(id, name, location, type)
{
    mMetrics = FileMetrics(QFileInfo(location));
}

QList<QPlainTextEdit*>&FileContext::editorList()
{
    return mEditors;
}

FileContext::~FileContext()
{
    removeAllEditors();
}

QString FileContext::codec() const
{
    return mCodec;
}

void FileContext::setCodec(const QString& codec)
{
    // TODO(JM) changing the codec must trigger conversion (not necessarily HERE)
    mCodec = codec;
}

const QString FileContext::caption()
{
    return name() + (isModified() ? "*" : "");
}

bool FileContext::isModified()
{
    return document() && document()->isModified();
}

void FileContext::save()
{
    if (isModified())
        save(location());
}

void FileContext::save(QString filePath)
{
    if (filePath == "")
        EXCEPT() << "Can't save without file name";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        EXCEPT() << "Can't open the file";
    mMetrics = FileMetrics();
    QTextStream out(&file);
    out.setCodec(mCodec.toLatin1().data());
    out << document()->toPlainText();
    out.flush();
    file.close();
    mMetrics = FileMetrics(QFileInfo(file));
    document()->setModified(false);
}

const FileMetrics& FileContext::metrics()
{
    return mMetrics;
}

const QList<QPlainTextEdit*> FileContext::editors() const
{
    return mEditors;
}

void FileContext::setLocation(const QString& _location)
{
    if (_location.isEmpty())
        EXCEPT() << "File can't be set to an empty location.";
    QFileInfo newLoc(_location);
    if (QFileInfo(location()) == newLoc)
        return; // nothing to do

    // TODO(JM) adapt parent group
    if (document())
        document()->setModified(true);
    FileSystemContext::setLocation(_location);
    mMetrics = FileMetrics(newLoc);
}

QIcon FileContext::icon()
{
    if (mMetrics.fileType() == FileType::Gms)
        return QIcon(":/img/gams-w");
    return QIcon(":/img/file-alt");
}

void FileContext::addEditor(QPlainTextEdit* edit)
{
    if (!edit)
        return;
    if (mEditors.contains(edit)) {
        mEditors.move(mEditors.indexOf(edit), 0);
        return;
    }

    mEditors.prepend(edit);
    if (mEditors.size() == 1) {
        document()->setParent(this);
        connect(document(), &QTextDocument::modificationChanged, this, &FileContext::modificationChanged, Qt::UniqueConnection);
        QTimer::singleShot(50, this, &FileContext::updateMarks);
    } else {
        edit->setDocument(document());
    }
    // TODO(JM) getMouseMove and -click for editor to enable link-clicking
    if (!edit->viewport()->hasMouseTracking()) {
        edit->viewport()->setMouseTracking(true);
    }
    edit->viewport()->installEventFilter(this);
    edit->installEventFilter(this);

    setFlag(FileSystemContext::cfActive);
}

void FileContext::addEditor(CodeEditor* edit)
{
    addEditor(static_cast<QPlainTextEdit*>(edit));
    connect(this, &FileContext::clearLineIcons, edit, &CodeEditor::clearLineIcons);
    connect(this, &FileContext::setLineIcon, edit, &CodeEditor::addLineIcon);
    if (!mTextMarks.isEmpty()) {
        edit->clearLineIcons();
        for (TextMark* mark: mTextMarks) {
            edit->addLineIcon(mark->line(), mark->icon());
        }
    }
}

void FileContext::editToTop(QPlainTextEdit* edit)
{
    addEditor(edit);
}

void FileContext::removeEditor(QPlainTextEdit* edit)
{
    int i = mEditors.indexOf(edit);
    if (i < 0)
        return;
    bool wasModified = isModified();
    mEditors.removeAt(i);
    if (mEditors.isEmpty()) {
        // After removing last editor: paste document-parency back to editor
        edit->document()->setParent(edit);
        disconnect(edit->document(), &QTextDocument::modificationChanged, this, &FileContext::modificationChanged);
        unsetFlag(FileSystemContext::cfActive);
        if (wasModified) emit changed(id());
    } else {
        edit->setDocument(document()->clone(edit));
    }
    edit->viewport()->removeEventFilter(this);
    edit->removeEventFilter(this);
}

void FileContext::removeAllEditors()
{
    auto editors = mEditors;
    for (auto editor : editors) {
        removeEditor(editor);
    }
    mEditors = editors;
}

bool FileContext::hasEditor(QPlainTextEdit* edit)
{
    return mEditors.contains(edit);
}

QTextDocument*FileContext::document()
{
    if (mEditors.isEmpty())
        return nullptr;
    return mEditors.first()->document();
}

void FileContext::load(QString codecName)
{
    if (!document())
        EXCEPT() << "There is no document assigned to the file " << location();

    QStringList codecNames = codecName.isEmpty() ? mDefaulsCodecs : QStringList() << codecName;
    QFile file(location());
    QTime tim(QTime::currentTime());
    qDebug() << "Start loading ...";
    if (!file.fileName().isEmpty() && file.exists()) {
        if (!file.open(QFile::ReadOnly | QFile::Text))
            EXCEPT() << "Error opening file " << location();
        mMetrics = FileMetrics();
        const QByteArray data(file.readAll());
        qDebug() << "Loaded " << (QTime::currentTime().elapsed()-tim.elapsed());
        QString text;
        QString nameOfUsedCodec;
        for (QString tcName: codecNames) {
            QTextCodec::ConverterState state;
            QTextCodec *codec = QTextCodec::codecForName(tcName.toLatin1().data());
            if (codec) {
                nameOfUsedCodec = tcName;
                text = codec->toUnicode(data.constData(), data.size(), &state);
                if (state.invalidChars == 0) {
                    qDebug() << "opened with codec " << nameOfUsedCodec;
                    break;
                }
                qDebug() << "Codec " << nameOfUsedCodec << " contains " << QString::number(state.invalidChars) << "invalid chars.";
            } else {
                qDebug() << "System doesn't contain codec " << nameOfUsedCodec;
                nameOfUsedCodec = QString();
            }
        }
        qDebug() << "Codec checked " << (QTime::currentTime().elapsed()-tim.elapsed());
        if (!nameOfUsedCodec.isEmpty()) {
            document()->setPlainText(text);
            mCodec = nameOfUsedCodec;
        }
        file.close();
        qDebug() << "Pasted do editor " << (QTime::currentTime().elapsed()-tim.elapsed());
        document()->setModified(false);
        mMetrics = FileMetrics(QFileInfo(file));
        QTimer::singleShot(100, this, &FileContext::updateMarks);
    }
    if (!mWatcher) {
        mWatcher = new QFileSystemWatcher(this);
        connect(mWatcher, &QFileSystemWatcher::fileChanged, this, &FileContext::onFileChangedExtern);
        mWatcher->addPath(location());
    }
}

void FileContext::jumpTo(const QTextCursor &cursor)
{
    if (mEditors.size()) {
        QPlainTextEdit* edit = mEditors.first();
        QTextCursor tc(cursor);
        tc.clearSelection();
        edit->setTextCursor(tc);
        emit openOrShow(this);
        // center line vertically
        int lines = edit->rect().bottom() / edit->cursorRect().height();
        int line = edit->cursorRect().bottom() / edit->cursorRect().height();
        int mv = line - lines/2;
        edit->verticalScrollBar()->setValue(edit->verticalScrollBar()->value()+mv);
    }
}

void FileContext::showToolTip(const TextMark& mark)
{
    if (mEditors.size() && !mark.textCursor().isNull()) {
        QPlainTextEdit* edit = mEditors.first();
        QTextCursor cursor(mark.textCursor());
        cursor.setPosition(cursor.anchor());
        QPoint pos = edit->cursorRect(cursor).bottomLeft();
        QString tip;
        emit requestErrorHint(mark.value(), tip);

        QToolTip::showText(edit->mapToGlobal(pos), tip, edit);
    }
}

TextMark* FileContext::generateTextMark(TextMark::Type tmType, int value, int line, int column, int size)
{
    TextMark* res = new TextMark(tmType);
    res->setPosition(this, line, column, size);
    res->setValue(value);
    mTextMarks.insertMulti(line, res);
    markLink(res);
    if (!res->icon().isNull()) {
        DEB() << "adding icon to " << name();
        emit setLineIcon(line, res->icon());
    }
    return res;
}

void FileContext::markLink(TextMark* mark)
{
    // TODO(JM) process marking in TextMark
    if (!mEditors.size() || !mark || mark->textCursor().isNull()) return;
    bool mod = document()->isModified();
    QPlainTextEdit *edit = mEditors.first();
    QTextCursor oldCur = QTextCursor(edit->document());
    QTextCursor cur = mark->textCursor();
    QTextCharFormat oldFormat = cur.charFormat();
    QTextCharFormat newFormat = oldFormat;
    if (mark->type() == TextMark::error) {
        newFormat.setUnderlineColor(Qt::red);
        newFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
        cur.setCharFormat(newFormat);
        cur.setPosition(mark->textCursor().selectionEnd());
        cur.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
        newFormat.setBackground(QColor(225,200,255));
        newFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        newFormat.setAnchorName(QString::number(mark->line()));
        cur.setCharFormat(newFormat);
    }
    if (mark->type() == TextMark::link) {
        newFormat.setForeground(Qt::blue);
        newFormat.setUnderlineColor(Qt::blue);
        newFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        newFormat.setToolTip(QString::number(mark->value()));
        newFormat.setAnchor(true);
        newFormat.setAnchorName(QString::number(mark->line()));
        cur.setCharFormat(newFormat);
    }
    edit->setTextCursor(oldCur);
    document()->setModified(mod);
}

void FileContext::removeTextMarks(TextMark::Type tmType)
{
    removeTextMarks(QSet<TextMark::Type>() << tmType);
}

void FileContext::removeTextMarks(QSet<TextMark::Type> tmTypes)
{
    QHash<int, TextMark*>::iterator i = mTextMarks.begin();
    while (i != mTextMarks.end()) {
        if (tmTypes.contains(i.value()->type()) || tmTypes.contains(TextMark::all)) {
            TextMark* tm = i.value();
            i = mTextMarks.erase(i);
            delete tm;
        } else {
            ++i;
        }
    }
    for (QPlainTextEdit* ed: mEditors) {
        ed->update(); // trigger delayed repaint
    }
}

bool FileContext::eventFilter(QObject* watched, QEvent* event)
{
    static QPoint clickPos;
    static QSet<QEvent::Type> evCheckMouse {QEvent::MouseButtonPress, QEvent::MouseButtonRelease, QEvent::MouseMove, QEvent::ToolTip};
    static QSet<QEvent::Type> evCheckKey {QEvent::KeyPress, QEvent::KeyRelease};

    if (!mEditors.size() || (watched != mEditors.first() && watched != mEditors.first()->viewport()))
        return FileSystemContext::eventFilter(watched, event);


    // TODO(JM) use updateLinkDisplay


    if (evCheckKey.contains(event->type())) {
        QKeyEvent *keyEv = static_cast<QKeyEvent*>(event);
        if (keyEv->modifiers() & Qt::ControlModifier) {
            mEditors.first()->viewport()->setCursor(mMouseOverLink ? Qt::PointingHandCursor : Qt::IBeamCursor);
        } else {
            mEditors.first()->viewport()->setCursor(Qt::IBeamCursor);
        }
        return FileSystemContext::eventFilter(watched, event);
    }

    if (evCheckMouse.contains(event->type())) {
        QPlainTextEdit* edit = mEditors.first();
        QPoint pos = (event->type() == QEvent::ToolTip)
                ? static_cast<QHelpEvent*>(event)->pos()
                : static_cast<QMouseEvent*>(event)->pos();
        QTextCursor cursor = edit->cursorForPosition(pos);
        if (event->type() == QEvent::MouseMove) {
            if (cursor.charFormat().isAnchor()) {
                mMouseOverLink = true;
                bool ctrl = true; //QApplication::keyboardModifiers() & Qt::ControlModifier;
                edit->viewport()->setCursor(ctrl ? Qt::PointingHandCursor : Qt::IBeamCursor);
            } else {
                mMouseOverLink = false;
                edit->viewport()->setCursor(Qt::IBeamCursor);
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            clickPos = pos;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            if ((clickPos-pos).manhattanLength() < 4) {
                for (TextMark* mark: mTextMarks.values(cursor.block().blockNumber())) {
                    if (mark->inColumn(cursor.positionInBlock())) {
                        mark->jumpToRefMark();
                        return true;
                    }
                }
            }
        } else if (event->type() == QEvent::ToolTip) {
            for (TextMark* mark: mTextMarks.values(cursor.block().blockNumber())) {
                if (mark->inColumn(cursor.positionInBlock())) {
                    mark->showToolTip();
                    return true;
                }
            }
        }
    }
    return FileSystemContext::eventFilter(watched, event);
}

bool FileContext::mouseOverLink()
{
    return mMouseOverLink;
}

void FileContext::modificationChanged(bool modiState)
{
    Q_UNUSED(modiState);
    if (location().isEmpty()) {
        DEB() << " log modified";
    }
    emit changed(id());
}

void FileContext::updateMarks()
{
    for (TextMark* mark: mTextMarks) {
        mark->updateCursor();
        markLink(mark);
    }
}

void FileContext::onFileChangedExtern(QString filepath)
{
    QFileInfo fi(filepath);
    FileMetrics::ChangeKind changeKind = mMetrics.check(fi);
    if (changeKind == FileMetrics::ckSkip) return;
    if (changeKind == FileMetrics::ckUnchanged) return;
    if (!fi.exists()) {
        // file has been renamed or deleted
        if (document()) document()->setModified();
        emit deletedExtern(id());
    }
    if (changeKind == FileMetrics::ckModified) {
        // file changed externally
        emit modifiedExtern(id());
    }
}

} // namespace studio
} // namespace gams
