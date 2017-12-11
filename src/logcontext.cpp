#include "logcontext.h"
#include "exception.h"
#include "filegroupcontext.h"

namespace gams {
namespace studio {

LogContext::LogContext(int id, QString name)
    : FileContext(id, name, "", FileSystemContext::Log)
{
    mDocument = new QTextDocument(this);
    mDocument->setDocumentLayout(new QPlainTextDocumentLayout(mDocument));
    mDocument->setDefaultFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

void LogContext::clearLog()
{
    document()->clear();
}

void LogContext::markOld()
{
    if (document() && !document()->isEmpty()) {
        QTextCursor cur(document());
        cur.movePosition(QTextCursor::End);
        QTextCharFormat oldFormat = cur.charFormat();
        QTextCharFormat newFormat = oldFormat;
        cur.insertBlock();
        cur.movePosition(QTextCursor::StartOfBlock);
        cur.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
        newFormat.setForeground(QColor(165,165,165));
        cur.setCharFormat(newFormat);
        cur.movePosition(QTextCursor::End);
        cur.setBlockCharFormat(oldFormat);
    }
}

QTextDocument* LogContext::document()
{
    return mDocument;
}

void LogContext::addEditor(QPlainTextEdit* edit)
{
    if (!edit) return;

    if (editorList().contains(edit)) {
        editorList().move(editorList().indexOf(edit), 0);
        return;
    }
    edit->setDocument(mDocument);
    FileContext::addEditor(edit);
}

void LogContext::removeEditor(QPlainTextEdit* edit)
{
    if (!edit) return;

    editorList().append(nullptr);
    FileContext::removeEditor(edit);
    editorList().removeLast();
}

void LogContext::setParentEntry(FileGroupContext* parent)
{
    if (parent){
        parent->setLogContext(this);
    } else {
        mParent->setLogContext(nullptr);
    }
    mParent = parent;
}

TextMark*LogContext::firstErrorMark()
{
    for (TextMark* mark: mTextMarks)
        if (mark->isErrorRef()) return mark;
    return nullptr;
}

void LogContext::addProcessData(QProcess::ProcessChannel channel, QString text)
{
    if (!mDocument)
        EXCEPT() << "no explicit document to add process data";
    ExtractionState state;
    QRegularExpression rEx("(\r?\n|\r\n?)");
    QStringList lines = text.split(rEx);
    if (!mLineBuffer.isEmpty()) {
        lines.replace(0,mLineBuffer+lines.at(0));
    }
    mLineBuffer = lines.last();
    lines.removeAt(lines.count()-1);
    for (QString line: lines) {
        QList<LinkData> marks;
        QString newLine = extractError(line, state, marks);
        if (state == FileContext::Exiting) {
            emit createErrorHint(mCurrentErrorHint.first, mCurrentErrorHint.second);
        }
        if (true || state != FileContext::Inside) {
            QList<int> scrollVal;
            QList<QTextCursor> cursors;
            for (QPlainTextEdit* ed: editors()) {
                if (ed->verticalScrollBar()->value() >= ed->verticalScrollBar()->maximum()-1){
                    scrollVal << 0;
                    cursors << QTextCursor();
                } else{
                    scrollVal << ed->verticalScrollBar()->value();
                    cursors << ed->textCursor();
                }
            }
            QTextCursor cursor(mDocument);
            cursor.movePosition(QTextCursor::End);
            int line = mDocument->lineCount()-1;
            cursor.insertText(newLine+"\n");
            for (LinkData mark: marks) {
                int size = (mark.size<=0) ? newLine.length()-mark.col : mark.size;
                TextMark* tm = generateTextMark(TextMark::link, mCurrentErrorHint.first, line, mark.col, size);
                tm->setRefMark(mark.textMark);
            }
            int i = 0;
            for (QPlainTextEdit* ed: editors()) {
                if (scrollVal[i] > 0) {
                    ed->verticalScrollBar()->setValue(scrollVal[i]);
                    ed->setTextCursor(cursors[i]);
                } else {
                    ed->verticalScrollBar()->setValue(ed->verticalScrollBar()->maximum());
                }
                ++i;
            }
            mDocument->setModified(false);
        }
    }
}

QString LogContext::extractError(QString line, FileContext::ExtractionState& state, QList<LogContext::LinkData>& marks)
{
    QString result;
    bool doubleLines = false;
    if (doubleLines) {
        result = "{"+line+"}\n";
    }
    if (!mInErrorDescription) {
        // look, if we find the start of an error
        QStringList parts = line.split(QRegularExpression("(\\[|]\\[|])"), QString::SkipEmptyParts);
        if (parts.size() > 1) {
            QRegularExpression errRX1("^([\\*\\-]{3} Error +(\\d+) in (.*)|ERR:\"([^\"]+)\",(\\d+),(\\d+)|LST:(\\d+)|FIL:\"([^\"]+)\",(\\d+),(\\d+))");
            TextMark* errMark = nullptr;
            bool errFound = false;
            for (QString part: parts) {
                bool ok;
                QRegularExpressionMatch match = errRX1.match(part);
                if (part.startsWith("***") || part.startsWith("---")) {
                    result += part;
                    int errNr = match.captured(2).toInt(&ok);
                    if (ok) {
                        mCurrentErrorHint.first = errNr;
                    } else {
                        mCurrentErrorHint.first = 0;
                    }
                    mCurrentErrorHint.second = "";

                } else if (part.startsWith("ERR")) {
                    QString fName = QDir::fromNativeSeparators(match.captured(4));
                    int line = match.captured(5).toInt()-1;
                    int col = match.captured(6).toInt()-1;
                    LinkData mark;
                    mark.col = result.indexOf(" ")+1;
                    result += " ";
                    mark.size = result.length() - mark.col - 1;
//                    mark.col = result.length()+1;
//                    result += QString("[ERR:%1]").arg(line+1);
//                    mark.size = result.length() - mark.col - 1;

                    FileContext *fc;
                    emit findFileContext(fName, &fc, parentEntry());
                    if (fc) {
                        mark.textMark = fc->generateTextMark(TextMark::error, mCurrentErrorHint.first, line, 0, col);
                        mMarkedContextList << fc;
                    }
                    errMark = mark.textMark;
                    marks << mark;
                    errFound = true;
                    mInErrorDescription = true;

                } else if (part.startsWith("LST")) {
                    QString fName = parentEntry()->lstFileName();
                    int line = match.captured(7).toInt()-1;
                    LinkData mark;
                    mark.col = result.length()+1;
                    result += QString("[LST:%1]").arg(line+1);
                    mark.size = result.length() - mark.col - 1;
                    FileContext *fc;
                    emit findFileContext(fName, &fc, parentEntry());
                    if (fc) {
                        mark.textMark = fc->generateTextMark((errFound ? TextMark::link : TextMark::none)
                                                             , mCurrentErrorHint.first, line, 0, 0);
                        mMarkedContextList << fc;
                        errFound = false;
                    } else {
                        result += line;
                        state = Outside;
                        break;
                    }
                    if (errMark) {
                        mark.textMark->setRefMark(errMark);
                        errMark->setRefMark(mark.textMark);
                    }
                    marks << mark;
                } else if (part.startsWith("FIL")) {
                    QString fName = QDir::fromNativeSeparators(match.captured(8));
                    LinkData mark;
                    int line = match.captured(9).toInt()-1;
                    int col = match.captured(10).toInt()-1;
                    mark.col = result.length()+1;
                    result += QString("[%1]").arg(QFileInfo(fName).suffix().toUpper());
                    mark.size = result.length() - mark.col - 1;
                    FileContext *fc;
                    emit findFileContext(fName, &fc, parentEntry());
                    if (fc) {
                        mark.textMark = fc->generateTextMark((errFound ? TextMark::link : TextMark::none)
                                                             , mCurrentErrorHint.first, line, 0, col);
                        mMarkedContextList << fc;
                        errFound = false;
                    } else {
                        result += line;
                        state = Outside;
                        break;
                    }
                    marks << mark;
                }
            }
            state = Entering;
        } else {
            result += line;
            state = Outside;
        }
    } else {
        if (line.startsWith(" ")) {
            // TODO(JM) get description from LST-file instead (-> there are more details)
            if (mCurrentErrorHint.second.isEmpty())
                mCurrentErrorHint.second += QString::number(mCurrentErrorHint.first)+'\t'+line.trimmed();
            else
                mCurrentErrorHint.second += "\n\t"+line.trimmed();
            state = Inside;
            result += line;
        } else {
            result += line;
            state = Exiting;
            mInErrorDescription = false;
        }
    }
    return result;
}

void LogContext::clearRecentMarks()
{
    for (FileContext* fc: mMarkedContextList) {
        fc->removeTextMarks(TextMark::all);
    }
    removeTextMarks(TextMark::all);
}

} // namespace studio
} // namespace gams
