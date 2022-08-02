/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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
#include <QDebug>
#include <QStandardItemModel>

#include "connecteditor.h"
#include "connectdatamodel.h"
#include "schemadefinitionmodel.h"
#include "theme.h"
#include "ui_connecteditor.h"

namespace gams {
namespace studio {
namespace connect {

ConnectEditor::ConnectEditor(const QString& connectDataFileName, QWidget *parent) :
    AbstractView(parent),
    ui(new Ui::ConnectEditor),
    mLocation(connectDataFileName)
{
    init();
}

bool ConnectEditor::init()
{
    ui->setupUi(this);
    setFocusProxy(ui->dataTreeView);

    qDebug() << "ConnectEditor::" << mLocation;

    mConnect = new Connect();
    QStringList schema = mConnect->getSchemaNames();

    QStandardItemModel* schemaItemModel = new QStandardItemModel( mConnect->getSchemaNames().size(), 1, this );
    QStandardItemModel* schemaHelpModel = new QStandardItemModel( mConnect->getSchemaNames().size(), 1, this );

    for(int row=0; row<mConnect->getSchemaNames().size(); row++) {
        QString str = mConnect->getSchemaNames().at(row);
        QStandardItem *helpitem = new QStandardItem();
        helpitem->setData( str, Qt::DisplayRole );
        schemaHelpModel->setItem(row, 0, helpitem);

        QStandardItem *item = new QStandardItem();
        item->setData( str, Qt::DisplayRole );
        item->setIcon(Theme::icon(":/%1/plus", true));
        item->setEditable(false);
        item->setSelectable(true);
        item->setTextAlignment(Qt::AlignLeft);
        item->setForeground(Theme::color(Theme::Syntax_embedded));
        schemaItemModel->setItem(row, 0, item);
    }

    ui->schemaControlListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->schemaControlListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->schemaControlListView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->schemaControlListView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->schemaControlListView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->schemaControlListView->setViewMode(QListView::ListMode);
    ui->schemaControlListView->setIconSize(QSize(16,16));
    ui->connectHSplitter->setSizes(QList<int>({15, 65, 20}));
    ui->schemaControlListView->setModel(schemaItemModel);
    ui->helpComboBox->setModel(schemaHelpModel);
//    ui->helpComboBox->setItemIcon(0, Theme::icon(":/solid/question-square", false));

    ConnectData* data = mConnect->loadDataFromFile(mLocation);
    qDebug() << data->str().c_str();

    mDataModel = new ConnectDataModel(mLocation, data, this);
    ui->dataTreeView->setModel( mDataModel );
    ui->dataTreeView->setEditTriggers(QAbstractItemView::DoubleClicked
                       | QAbstractItemView::SelectedClicked
                       | QAbstractItemView::EditKeyPressed
                       | QAbstractItemView::AnyKeyPressed );
//    ui->dataTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dataTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->dataTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->dataTreeView->setItemsExpandable(true);
    ui->dataTreeView->setAutoScroll(true);
    updateDataColumnSpan();
    ui->dataTreeView->expandAll();
    ui->dataTreeView->resizeColumnToContents(0);
    ui->dataTreeView->resizeColumnToContents(1);
    headerRegister(ui->dataTreeView->header());

    SchemaDefinitionModel* defmodel = new SchemaDefinitionModel(mConnect, mConnect->getSchemaNames().first(), this);
    ui->helpTreeView->setModel( defmodel );
    ui->helpTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->helpTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->helpTreeView->setItemsExpandable(true);
    ui->helpTreeView->expandAll();
    ui->helpTreeView->resizeColumnToContents(0);
    ui->helpTreeView->resizeColumnToContents(1);
    ui->helpTreeView->resizeColumnToContents(2);
    ui->helpTreeView->resizeColumnToContents(3);
    ui->helpTreeView->resizeColumnToContents(4);
    headerRegister(ui->helpTreeView->header());

//    connect(ui->schemaControlListView, &QListView::clicked, this, &ConnectEditor::schemaClicked);
    connect(ui->schemaControlListView, &QListView::doubleClicked, this, &ConnectEditor::schemaDoubleClicked);

    connect(ui->helpComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        defmodel->loadSchemaFromName( schemaHelpModel->data( schemaHelpModel->index(index,0) ).toString() );
    });
    connect(defmodel, &ConnectDataModel::modelReset, [this]() {
        ui->dataTreeView->expandAll();
        ui->dataTreeView->resizeColumnToContents(0);
        ui->dataTreeView->resizeColumnToContents(1);
    });
    connect(defmodel, &SchemaDefinitionModel::modelReset, [this]() {
        ui->helpTreeView->expandAll();
        ui->helpTreeView->resizeColumnToContents(0);
        ui->helpTreeView->resizeColumnToContents(1);
        ui->helpTreeView->resizeColumnToContents(2);
        ui->helpTreeView->resizeColumnToContents(3);
        ui->helpTreeView->resizeColumnToContents(4);
    });
    return true;
}

ConnectEditor::~ConnectEditor()
{
    delete ui;
    if (mDataModel)
        delete mDataModel;
    if (mConnect)
        delete mConnect;
}

//void ConnectEditor::schemaClicked(const QModelIndex &modelIndex)
//{
//    qDebug() << "clikced row=" << modelIndex.row() << ", col=" << modelIndex.column();
//}

void ConnectEditor::schemaDoubleClicked(const QModelIndex &modelIndex)
{
    qDebug() << "doubseclikced row=" << modelIndex.row() << ", col=" << modelIndex.column()
             << ui->schemaControlListView->model()->data( modelIndex ).toString();

    QStringList strlist;
    strlist << ui->schemaControlListView->model()->data( modelIndex ).toString();

    mDataModel->addFromSchema( mConnect->createDataHolder(strlist) );
    updateDataColumnSpan();
    ui->dataTreeView->expandAll();
    ui->dataTreeView->resizeColumnToContents(0);
    ui->dataTreeView->resizeColumnToContents(1);
}

void ConnectEditor::updateDataColumnSpan()
{
    qDebug() << "updateColumnSpan";
    ui->dataTreeView->setFirstColumnSpanned(0, ui->dataTreeView->rootIndex(), true);
}

}
}
}
