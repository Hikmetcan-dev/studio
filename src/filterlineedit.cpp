#include "filterlineedit.h"
#include "theme.h"
#include "logger.h"
#include <QAction>
#include <QToolButton>
#include <QPainter>
#include <QHBoxLayout>
#include <QSpacerItem>

namespace gams {
namespace studio {

FilterLineEdit::FilterLineEdit(QWidget *parent): QLineEdit(parent)
{
    init();
}

FilterLineEdit::FilterLineEdit(const QString &contents, QWidget *parent): QLineEdit(contents, parent)
{
    init();
}

const QRegExp &FilterLineEdit::regExp() const
{
    return mRegExp;
}

void FilterLineEdit::hideColumnButton(bool allColumns)
{
    nextButtonState(mAllColButton, allColumns ? 1 : 0);
    mAllColButton->setVisible(false);
}

bool FilterLineEdit::allColumns()
{
    return buttonState(mAllColButton);
}

void FilterLineEdit::init()
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding));
    lay->setContentsMargins(0,0,3,0);
    lay->setSpacing(0);

    mClearButton = createButton(QStringList() << ":/img/tremove", QStringList() << "Clear");
    connect(mClearButton, &QToolButton::clicked, this, [this](){ clear(); });
    lay->addWidget(mClearButton);

    mExactButton = createButton(QStringList() << ":/img/tpart" << ":/img/twhole",
                                QStringList() << "Only allow exact matches" << "Allow substring matches");
    connect(mExactButton, &QToolButton::clicked, this, [this](){ nextButtonState(mExactButton); });
    lay->addWidget(mExactButton);

    mRegExButton = createButton(QStringList() << ":/img/trex-off" << ":/img/trex-on",
                                QStringList() << "Wildcard matching" << "Regular expression matching");
    connect(mRegExButton, &QToolButton::clicked, this, [this](){ nextButtonState(mRegExButton); });
    lay->addWidget(mRegExButton);

    mAllColButton = createButton(QStringList() << ":/img/tcol-one" << ":/img/tcol-all",
                                 QStringList() << "Search in key column only" << "Search in all columns");
    connect(mAllColButton, &QToolButton::clicked, this, [this](){
        nextButtonState(mAllColButton);
        emit columnScopeChanged();
    });
    lay->addWidget(mAllColButton);

    setLayout(lay);
    connect(this, &FilterLineEdit::textChanged, this, [this](){ updateRegExp(); });
    updateRegExp();
}

void FilterLineEdit::updateRegExp()
{
    mClearButton->setVisible(!text().isEmpty());
    QString rawText = buttonState(mRegExButton) ? text() : QRegExp::escape(text());
    QString filter = text().isEmpty() ? QString() : buttonState(mExactButton) ? '^' + rawText + '$'
                                                                              : rawText;
    filter.replace("\\*", ".*");
    filter.replace("\\?", ".");
    mRegExp = QRegExp(filter);
    mRegExp.setCaseSensitivity(Qt::CaseInsensitive);
    emit regExpChanged(mRegExp);
}

QToolButton *FilterLineEdit::createButton(const QStringList &iconPaths, const QStringList &toolTips)
{
    if (iconPaths.isEmpty()) {
        DEB() << "At least one icon needed";
        return nullptr;
    }
    if (iconPaths.size() != toolTips.size()) {
        DEB() << "Count of icons and tool tips differ";
        return nullptr;
    }
    QToolButton *button = new QToolButton(this);
    button->setIconSize(QSize(height()/2,height()/2));
    button->setContentsMargins(0,0,0,0);
    button->setStyleSheet("border:none;");
    button->setCursor(Qt::PointingHandCursor);
    button->setIcon(Theme::instance()->icon(iconPaths.at(0)));
    button->setToolTip(toolTips.at(0));
    button->setProperty("icons", iconPaths);
    button->setProperty("tips", toolTips);
    return button;
}

int FilterLineEdit::nextButtonState(QToolButton *button, int forceState)
{
    bool ok;
    int state = button->property("state").toInt(&ok);
    if (!ok) state = 0;
    QStringList icons = button->property("icons").toStringList();
    QStringList tips = button->property("tips").toStringList();
    if (icons.isEmpty()) return -1;
    if (forceState < 0)
        state = (state+1) % icons.size();
    else
        state = forceState % icons.size();
    button->setIcon(Theme::instance()->icon(icons.at(state)));
    button->setToolTip(tips.at(state));
    if (icons.size() > 1) button->setProperty("state", state);
    updateRegExp();
    return state;
}

int FilterLineEdit::buttonState(QToolButton *button)
{
    bool ok;
    int state = button->property("state").toInt(&ok);
    if (!ok) state = 0;
    return state;
}

} // namespace studio
} // namespace gams
