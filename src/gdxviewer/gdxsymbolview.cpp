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
#include "gdxsymbolview.h"
#include "ui_gdxsymbolview.h"
#include "gdxsymbolheaderview.h"
#include "gdxsymbol.h"
#include "columnfilter.h"

#include <QClipboard>

namespace gams {
namespace studio {
namespace gdxviewer {

GdxSymbolView::GdxSymbolView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GdxSymbolView)
{
    ui->setupUi(this);

    //create context menu
    QAction* cpComma = mContextMenu.addAction("Copy (comma-separated)\tCtrl+C", [this]() { copySelectionToClipboard(","); });
    cpComma->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    cpComma->setShortcutVisibleInContextMenu(true);
    cpComma->setShortcutContext(Qt::WidgetShortcut);
    ui->tvListView->addAction(cpComma);

    QAction* cpTab = mContextMenu.addAction("Copy (tab-separated)", [this]() { copySelectionToClipboard("\t"); }, QKeySequence("Ctrl+Shift+C"));
    cpTab->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    cpTab->setShortcutVisibleInContextMenu(true);
    ui->tvListView->addAction(cpTab);

    mContextMenu.addSeparator();

    QAction* selectAll = mContextMenu.addAction("Select All\tCtrl+A", ui->tvListView, &QTableView::selectAll);
    selectAll->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    selectAll->setShortcutVisibleInContextMenu(true);
    ui->tvListView->addAction(selectAll);

    //create header
    GdxSymbolHeaderView* headerView = new GdxSymbolHeaderView(Qt::Horizontal);
    headerView->setEnabled(false);

    ui->tvListView->setHorizontalHeader(headerView);
    ui->tvListView->setSortingEnabled(true);
    ui->tvListView->horizontalHeader()->setSortIndicatorShown(true);
    ui->tvListView->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    ui->tvListView->horizontalHeader()->setSectionsClickable(true);
    ui->tvListView->horizontalHeader()->setSectionsMovable(true);
    ui->tvListView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tvListView->horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &GdxSymbolView::showColumnFilter);
    connect(ui->cbSqueezeDefaults, &QCheckBox::toggled, this, &GdxSymbolView::toggleSqueezeDefaults);
    connect(ui->pbResetSortFilter, &QPushButton::clicked, this, &GdxSymbolView::resetSortFilter);

    ui->tvListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvListView, &QTableView::customContextMenuRequested, this, &GdxSymbolView::showContextMenu);
}

GdxSymbolView::~GdxSymbolView()
{
    delete ui;
}

void GdxSymbolView::showColumnFilter(QPoint p)
{
    int column = ui->tvListView->horizontalHeader()->logicalIndexAt(p);
    if(mSym->isLoaded() && column>=0 && column<mSym->dim()) {
        QMenu m(this);
        m.addAction(new ColumnFilter(mSym, column, this));
        m.exec(ui->tvListView->mapToGlobal(p));
    }
}

void GdxSymbolView::toggleSqueezeDefaults(bool checked)
{
    if (mSym) {
        ui->tvListView->setUpdatesEnabled(false);
        if (checked) {
            for (int i=0; i<GMS_VAL_MAX; i++) {
                if (mSym->isAllDefault(i))
                    ui->tvListView->setColumnHidden(mSym->dim()+i, true);
                else
                    ui->tvListView->setColumnHidden(mSym->dim()+i, false);
            }
        }
        else {
            for(int i=0; i<GMS_VAL_MAX; i++)
                ui->tvListView->setColumnHidden(mSym->dim()+i, false);
        }
        ui->tvListView->setUpdatesEnabled(true);
    }
}

void GdxSymbolView::resetSortFilter()
{
    if(mSym) {
        mSym->resetSortFilter();
        ui->tvListView->horizontalHeader()->restoreState(mInitialHeaderState);
    }
    ui->cbSqueezeDefaults->setChecked(false);
}

void GdxSymbolView::refreshView()
{
    if(!mSym)
        return;
    if(mSym->isLoaded())
        mSym->filterRows();
}

GdxSymbol *GdxSymbolView::sym() const
{
    return mSym;
}

void GdxSymbolView::setSym(GdxSymbol *sym)
{
    mSym = sym;
    if (mSym->recordCount()>0) //enable controls only for symbols that have records, otherwise it does not make sense to filter, sort, etc
        connect(mSym, &GdxSymbol::loadFinished, this, &GdxSymbolView::enableControls);
    ui->tvListView->setModel(mSym);
    refreshView();
}

void GdxSymbolView::copySelectionToClipboard(QString separator)
{
    if (!ui->tvListView->model())
        return;
    // row -> column -> QModelIndex
    QMap<int, QMap<int, QString>> sortedSelection;
    QModelIndexList selection = ui->tvListView->selectionModel()->selection().indexes();
    if (selection.isEmpty())
        return;

    int minRow = std::numeric_limits<int>::max();
    int maxRow = std::numeric_limits<int>::min();
    int minCol = std::numeric_limits<int>::max();
    int maxCol = std::numeric_limits<int>::min();

    for (QModelIndex idx : selection) {
        int currentRow = idx.row();
        int currentCol = idx.column();
        currentCol = ui->tvListView->horizontalHeader()->visualIndex(currentCol);
        QString currenText = idx.data().toString();
        if (currenText.contains(separator)) {
            if (currenText.contains("\'"))
                currenText = "\"" + currenText + "\"";
            else
                currenText = "\'" + currenText + "\'";
        }
        sortedSelection[currentRow][currentCol] = currenText;

        minRow = qMin(minRow, currentRow);
        maxRow = qMax(maxRow, currentRow);
        minCol = qMin(minCol, currentCol);
        maxCol = qMax(maxCol, currentCol);
    }

    QStringList sList;
    for(int r=minRow; r<maxRow+1; r++) {
        for(int c=minCol; c<maxCol+1; c++)
            sList << sortedSelection[r][c] << separator;
        sList.pop_back(); // remove last separator
        sList << "\n";
    }
    sList.pop_back();  // remove last newline

    QClipboard* clip = QApplication::clipboard();;
    clip->setText(sList.join(""));
}

void GdxSymbolView::showContextMenu(QPoint p)
{
    mContextMenu.exec(ui->tvListView->mapToGlobal(p));
}

void GdxSymbolView::enableControls()
{
    ui->tvListView->horizontalHeader()->setEnabled(true);
    mInitialHeaderState = ui->tvListView->horizontalHeader()->saveState();
    if(mSym->type() == GMS_DT_VAR || mSym->type() == GMS_DT_EQU)
        ui->cbSqueezeDefaults->setEnabled(true);
    else
        ui->cbSqueezeDefaults->setEnabled(false);
    ui->pbResetSortFilter->setEnabled(true);
}

} // namespace gdxviewer
} // namespace studio
} // namespace gams
