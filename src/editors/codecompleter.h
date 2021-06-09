#ifndef GAMS_STUDIO_SYNTAX_CODECOMPLETER_H
#define GAMS_STUDIO_SYNTAX_CODECOMPLETER_H

#include <QListView>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QTextBlock>
#include <QPlainTextEdit>
#include <QMetaEnum>

namespace gams {
namespace studio {

enum CodeCompleterCasing {
    caseCamel,
    caseLower,
    caseUpper,
    caseDynamic
};

enum CodeCompleterType {
    cc_None     = 0x00000000,

    ccDcoStrt   = 0x00000001, // DCO (starter and standalone)
    ccDcoEnd    = 0x00000002, // DCO (ender, e.g. $offText)
    ccSubDcoA   = 0x00000004, // sub DCO of $abort
    ccSubDcoC   = 0x00000008, // sub DCO of $call
    ccSubDcoE   = 0x00000010, // sub DCO of $eval

    cc_Dco      = 0x0000001F, // all DCOs
    cc_SubDco   = 0x0000001C, // all sub DCOs

    ccSysDat    = 0x00000020, // system data
    ccSysSufR   = 0x00000040, // system suffix run-time
    ccSysSufC   = 0x00000080, // system suffix compile-time

    ccDecl      = 0x00000100, // declarations
    ccDeclS     = 0x00000200, // declaration: Set
    ccDeclV     = 0x00000400, // declaration: Variable
    ccDeclT     = 0x00000800, // declaration: Table
    ccDeclAddV  = 0x00001000, // declaration additions for "variable"
    ccDeclAddS  = 0x00002000, // declaration additions for "set"
    ccRes       = 0x00004000, // other reserved words
    ccResEnd    = 0x00008000, // embedded end
    cc_Res      = 0x0000FF00, // all reserved words

    ccOpt       = 0x00010000, // options
    ccMod       = 0x00020000, // models
    ccSolve     = 0x00040000, // solve
    ccExec      = 0x00080000, // execute additions

    cc_Start    = 0x00003F81, // all starting keywords

    cc_All      = 0x7FFFFFFF
};

class CodeCompleterModel : public QAbstractListModel
{
public:


private:
    QStringList mData;
    QStringList mDescription;
    QList<int> mDescriptIndex;
    QMap<int, CodeCompleterType> mType;
    CodeCompleterCasing mCasing;
    int mDollarGroupRow = -1;
    int mPercentGroupRow = -1;
    Q_OBJECT
public:
    CodeCompleterModel(QObject *parent = nullptr);
    ~CodeCompleterModel() override {}
    void setCasing(CodeCompleterCasing casing);
    CodeCompleterCasing casing() { return mCasing; }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int dollarGroupRow() { return mDollarGroupRow; }
    int percentGroupRow() { return mPercentGroupRow; }

private:
    void initData();
    void addDynamicData();
};

class FilterCompleterModel : public QSortFilterProxyModel
{
    int mTypeFilter = 0;
    int mSubType = 0;
    bool mNeedDot = true;
    bool mEmpty = true;
    int mDollarGroupRow = -1;
    int mPercentGroupRow = -1;
    Q_OBJECT
public:
    FilterCompleterModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
     ~FilterCompleterModel() override {}
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    void setGroupRows(int dollarRow, int percentRow);
    bool isGroupRow(int row);
    bool test(int type, int flagPattern) const;
    void setTypeFilter(int completerTypeFilter, int subType, bool needDot);
    int typeFilter() const { return mTypeFilter; }
    void setFilterText(QString filterText);
    void setEmpty(bool isEmpty);
};

class CodeCompleter : public QListView
{
    Q_OBJECT
public:
    CodeCompleter(QPlainTextEdit *parent = nullptr);
    ~CodeCompleter() override;
    void setCodeEdit(QPlainTextEdit *edit);
    QPlainTextEdit* codeEdit() { return mEdit; }
    void updateFilter(int posInBlock = -1, QString line = QString());
    void updateDynamicData(QStringList symbols);
    int rowCount();
    void ShowIfData();
    void setCasing(CodeCompleterCasing casing);
    QString filterText() const;
    int typeFilter() const;
    QStringList splitTypes(int filter = -1);
    void setDebugMode(bool debug);

signals:
    void scanSyntax(QTextBlock block, QMap<int, QPair<int,int>> &blockSyntax);

public slots:
    void setVisible(bool visible) override;

protected:
    bool event(QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void focusOutEvent(QFocusEvent *event) override;
    void actionEvent(QActionEvent *event) override;

private:
    void insertCurrent(bool equalPartOnly = false);
    int findBound(int pos, const QString &nextTwo, int good, int look);
    int findFilterRow(const QString &text, int top, int bot);
    void updateFilterFromSyntax(const QPair<int, int> &syntax, int dcoFlavor, const QString &line, int pos);
    QPair<int, int> getSyntax(QTextBlock block, int pos, int &dcoFlavor);

private:
    QPlainTextEdit *mEdit;
    CodeCompleterModel *mModel;
    FilterCompleterModel *mFilterModel;
    QString mFilterText;
    QString mPreferredText;
    bool mDebug = false;
};

} // namespace studio
} // namespace gams

#endif // GAMS_STUDIO_SYNTAX_CODECOMPLETER_H
