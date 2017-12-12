#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QtGui>
#include "syntaxformats.h"
#include "syntaxdeclaration.h"

namespace gams {
namespace studio {

class ErrorHighlighter : public QSyntaxHighlighter
{
public:
    struct ErrorMark {
        ErrorMark() {}

    };

    ErrorHighlighter(QTextDocument *parent = 0);
    void highlightBlock(const QString &text);

private slots:
    void docBlockCountChanged(int newCount);
    void docContentsChange(int from, int removed, int added);

private:
    QTextBlock mTestBlock;
    QList<ErrorMark*> mErrors;
};

class SyntaxHighlighter : public ErrorHighlighter
{
public:
    SyntaxHighlighter(QTextDocument *parent = 0);
    ~SyntaxHighlighter();

    void highlightBlock(const QString &text);

private:
    SyntaxAbstract *getSyntax(SyntaxState state) const;
    int getStateIdx(SyntaxState state) const;

private:
    typedef int StateIndex;
    typedef int CodeIndex;
    typedef QPair<StateIndex, CodeIndex> StateCode;
    typedef QList<SyntaxAbstract*> States;
    typedef QList<StateCode> Codes;

    void addState(SyntaxAbstract* syntax, CodeIndex ci = 0);
    int addCode(StateIndex si, CodeIndex ci);
    int getCode(CodeIndex code, SyntaxStateShift shift, StateIndex state);

    States mStates;
    Codes mCodes;
};

} // namespace studio
} // namespace gams

#endif // SYNTAXHIGHLIGHTER_H
