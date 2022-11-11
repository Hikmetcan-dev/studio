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
#include <QColor>
#include <QApplication>
#include <QPalette>
#include <QStringListModel>
#include <QMimeData>
#include <QDir>

#include "connectdatamodel.h"
#include "theme.h"
#include "commonpaths.h"
#include "exception.h"

namespace gams {
namespace studio {
namespace connect {

const QString ConnectDataModel::TooltipStrHeader = QString("<html><head/><body>");
const QString ConnectDataModel::TooltipStrFooter = QString("</body></html>");
const QString ConnectDataModel::TooltipOpenedBoldStr = QString("<span style=' font-weight:600;'>");
const QString ConnectDataModel::TooltipClosedBoldStr = QString("</span>");

ConnectDataModel::ConnectDataModel(const QString& filename,  Connect* c, QObject *parent)
    : QAbstractItemModel{parent},
      mOnlyRequriedAttributesAdded(false),
      mItemIDCount(0),
      mLocation(filename),
      mConnect(c)
{
    try {
        mConnectData = mConnect->loadDataFromFile(mLocation);
    } catch (std::exception &e) {
        EXCEPT() << e.what();
    }

    qDebug() << mConnectData->str().c_str();
    setupTreeItemModelData();
}

ConnectDataModel::~ConnectDataModel()
{
    if (mRootItem)
        delete mRootItem;
}

QVariant ConnectDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::EditRole: {

    }
    case Qt::DisplayRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        if (index.column()==(int)DataItemColumn::SchemaType || index.column()==(int)DataItemColumn::AllowedValue) {
           return  QVariant(item->data(index.column()).toStringList());
        } else if (index.column()==(int)DataItemColumn::Key) {
                  QModelIndex checkstate_index = index.sibling(index.row(),(int)DataItemColumn::CheckState );
                  if (checkstate_index.data(Qt::DisplayRole).toInt()==(int)DataCheckState::ListItem) {
                     return QVariant(index.row());
                  } else if (checkstate_index.data(Qt::DisplayRole).toInt()==(int)DataCheckState::ListAppend) {
                      QString key = item->data(Qt::DisplayRole).toString();
                      if (key.contains("["))
                          return QVariant(key.left(item->data(Qt::DisplayRole).toString().indexOf("[")));
                  }
                  return item->data(index.column());
        } else if (index.column()==(int)DataItemColumn::ElementID) {
                 return QVariant(item->id());
        } else if (index.column()==(int)DataItemColumn::SchemaKey) {
                 return QVariant(item->data(index.column()).toStringList().join(":"));
        } else if (index.column()==(int)DataItemColumn::Unknown) {
                  return QVariant(item->data(index.column()));
        } else {
            return item->data(index.column());
        }
    }
    case Qt::ForegroundRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        if (item->data((int)DataItemColumn::Unknown).toBool()) {
            if (index.column() == (int)DataItemColumn::Key &&
               !item->parentItem()->data((int)DataItemColumn::Unknown).toBool())
                return  QVariant::fromValue(Theme::color(Theme::Normal_Red));
        }

        int state = item->data( (int)DataItemColumn::CheckState ).toInt();
        if (state==(int)DataCheckState::Root)
            return  QVariant::fromValue(Theme::color(Theme::Syntax_declaration));
        else if (state==(int)DataCheckState::SchemaName)
                return  QVariant::fromValue(Theme::color(Theme::Normal_Green));
        else if (state==(int)DataCheckState::ListItem || state==(int)DataCheckState::MapAppend || state==(int)DataCheckState::ListAppend)
                 return  QVariant::fromValue(Theme::color(Theme::Disable_Gray));
        else if (state==(int)DataCheckState::ElementMap || state==(int)DataCheckState::ElementKey)
            return  QVariant::fromValue(Theme::color(Theme::Syntax_keyword));
        else if (index.column()==(int)DataItemColumn::Value)
            return  QVariant::fromValue(Theme::color(Theme::Syntax_keyword));
        else
            return  QVariant::fromValue(QApplication::palette().color(QPalette::Text));
    }
    case Qt::BackgroundRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        ConnectDataItem *parentItem = item->parentItem();
        if (parentItem == mRootItem ) {
            return QVariant::fromValue(QApplication::palette().color(QPalette::Window));
        } else {
            if (item->data( (int)DataItemColumn::CheckState ).toInt() <= (int)DataCheckState::ListItem   ||
                item->data( (int)DataItemColumn::CheckState ).toInt() == (int)DataCheckState::ListAppend ||
                item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::MapAppend       )
                return QVariant::fromValue(QGuiApplication::palette().color(QPalette::Window));
            else
                return QVariant::fromValue(QApplication::palette().color(QPalette::Base));
        }
    }
    case Qt::UserRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        return QVariant(item->id());
    }
    case Qt::ToolTipRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        if (item->data(index.column()).toBool()) {
            ConnectDataItem *parentItem = item->parentItem();
            QModelIndex data_index = index.sibling(index.row(),(int)DataItemColumn::Key );
            if (parentItem != mRootItem) {
                if (index.column()==(int)DataItemColumn::Delete) {
                    return QVariant( QString("%1 Delete %2%3%4 and all children, if there is any%5")
                                     .arg( TooltipStrHeader,TooltipOpenedBoldStr,data_index.data(Qt::DisplayRole).toString(),TooltipClosedBoldStr,TooltipStrFooter ) );
                } else if (index.column()==(int)DataItemColumn::MoveDown) {
                              return QVariant( QString("%1 Move %2%3%4 and all it children down %5")
                                               .arg( TooltipStrHeader,TooltipOpenedBoldStr,data_index.data(Qt::DisplayRole).toString(),TooltipClosedBoldStr,TooltipStrFooter ) );
                } else if (index.column()==(int)DataItemColumn::MoveUp) {
                              return QVariant( QString("%1 Move %2%3%4 and all it children up %5")
                                               .arg( TooltipStrHeader,TooltipOpenedBoldStr,data_index.data(Qt::DisplayRole).toString(),TooltipClosedBoldStr,TooltipStrFooter ) );
                } else  if (index.column()==(int)DataItemColumn::Key) {
                           QVariant data = index.data(index.column());
                           if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::SchemaName) {
                               if (!item->data( (int)DataItemColumn::Unknown ).toBool() ) {
                                   return QVariant( QString("%1 Show help for %2%3%4 schema %5")
                                                    .arg( TooltipStrHeader,TooltipOpenedBoldStr,data_index.data(Qt::DisplayRole).toString(),TooltipClosedBoldStr,TooltipStrFooter) );
                               } else {
                                   return QVariant( QString("%1 Unknown schema name %2%3%4.<br/>Note that name is case-sensitive.<br/>See %2%5%4 location for known schema definition.%6")
                                                    .arg( TooltipStrHeader,TooltipOpenedBoldStr,data.toString(),TooltipClosedBoldStr,QDir::cleanPath(CommonPaths::systemDir()+QDir::separator()+ CommonPaths::gamsConnectSchemaDir()),TooltipStrFooter ) );
                               }
                           } else if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ListAppend) {
                                      if (item->parentItem() && item->parentItem()->data((int)DataItemColumn::Unknown ).toBool())
                                          return QVariant( QString("%1 Unable to add element to the list of the unknown %2%3%4<br/>Check schema definition for valid attribute name or name of its parent.<br/>Note that name is case-sensitive %5")
                                                           .arg( TooltipStrHeader,TooltipOpenedBoldStr,data.toString(),TooltipClosedBoldStr,TooltipStrFooter) );
                                     return QVariant( QString("%1 Add element to the list of %2%3%4 %5")
                                                      .arg( TooltipStrHeader,TooltipOpenedBoldStr,data.toString(),TooltipClosedBoldStr,TooltipStrFooter ) );
                           } else if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::MapAppend) {
                                      return QVariant( QString("%1 Add element to %2%3%4 dict %5")
                                                       .arg( TooltipStrHeader,TooltipOpenedBoldStr,data.toString(),TooltipClosedBoldStr,TooltipStrFooter ) );
                           } else if (item->data( (int)DataItemColumn::Unknown).toBool()) {
                                     return QVariant( QString("%1 %2%3%4 attribute is unknown.<br/>Check schema definition for valid attribute name or name of its parent.<br/>Note that name is case-sensitive.%5")
                                                           .arg( TooltipStrHeader,TooltipOpenedBoldStr,data.toString(),TooltipClosedBoldStr,TooltipStrFooter ));
                           }
                }
            }
        }
        break;
    }
    case Qt::DecorationRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        QModelIndex checkstate_index = index.sibling(index.row(),(int)DataItemColumn::CheckState );
        QModelIndex unknown_index = index.sibling(index.row(),(int)DataItemColumn::Unknown );
       if (index.column()==(int)DataItemColumn::Key) {
           if (checkstate_index.data(Qt::DisplayRole).toInt()==(int)DataCheckState::SchemaName) {
               if (unknown_index.data(Qt::DisplayRole).toBool())
                  return QVariant::fromValue(Theme::icon(":/solid/kill"));
               else
                  return QVariant::fromValue(Theme::icon(":/solid/question"));
           } else if (checkstate_index.data(Qt::DisplayRole).toInt()==(int)DataCheckState::ListAppend ||
                      checkstate_index.data(Qt::DisplayRole).toInt()==(int)DataCheckState::MapAppend     ) {
               return QVariant::fromValue(Theme::icon(":/solid/plus"));
           }
       } else if (index.column()==(int)DataItemColumn::Delete) {
                 if (item->data(index.column()).toBool())
                     return QVariant::fromValue(Theme::icon(":/solid/delete-all"));
       } else if (index.column()==(int)DataItemColumn::MoveDown) {
                  if (item->data(index.column()).toBool())
                      return QVariant::fromValue(Theme::icon(":/solid/move-down"));
       } else if (index.column()==(int)DataItemColumn::MoveUp) {
                  if (item->data(index.column()).toBool())
                      return QVariant::fromValue(Theme::icon(":/solid/move-up"));
       }
        break;
    }
    default:
         break;
    }
    return QVariant();

}

Qt::ItemFlags ConnectDataModel::flags(const QModelIndex &index) const
{
    ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    } else if (index.column()==(int)DataItemColumn::Key) {
               if (item->data((int)DataItemColumn::CheckState).toInt()== (int)DataCheckState::ElementMap ||
                   item->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ElementKey    )
                   return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
               else
                   return Qt::ItemIsDropEnabled | Qt::NoItemFlags;
    } else if (index.column()==(int)DataItemColumn::Value) {
              if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ElementKey   ||
                  item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ElementMap      )
                  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
              else if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ElementValue)
                      return Qt::ItemIsEditable | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
              else if (item->data( (int)DataItemColumn::CheckState ).toInt()<=(int)DataCheckState::ElementKey ||
                       item->data( (int)DataItemColumn::CheckState ).toInt()>=(int)DataCheckState::ListAppend)
                     return   Qt::NoItemFlags;
              else
                    return QAbstractItemModel::flags(index);
    } else if (index.column()>(int)DataItemColumn::CheckState) {
               return Qt::NoItemFlags;
    } else {
        return QAbstractItemModel::flags(index);
    }
}

bool ConnectDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QVector<int> roles;
    switch (role) {
    case Qt::EditRole: {
        roles = { Qt::EditRole };
        ConnectDataItem *item = getItem(index);
        bool result = item->setData(index.column(), value);
        if (result)
            emit dataChanged(index, index, roles);
        return result;
    }
    default:
        break;
    }
    return false;
}

QVariant ConnectDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return mRootItem->data(section);

    return QVariant();
}

QModelIndex ConnectDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ConnectDataItem *parentItem = getItem(parent);
    ConnectDataItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ConnectDataModel::indexForTreeItem(ConnectDataItem *item)
{
    return createIndex(item->childNumber(), 0, item);
}

QModelIndex ConnectDataModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ConnectDataItem *childItem = static_cast<ConnectDataItem*>(index.internalPointer());
    ConnectDataItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);

}

int ConnectDataModel::rowCount(const QModelIndex &parent) const
{
    ConnectDataItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<ConnectDataItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ConnectDataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<ConnectDataItem*>(parent.internalPointer())->columnCount();
    else
        return mRootItem->columnCount();
}

ConnectDataItem *ConnectDataModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return mRootItem;
}

void ConnectDataModel::insertItem(int position, ConnectDataItem *item, const QModelIndex &parent)
{
    ConnectDataItem* parentItem = getItem(parent);

    beginInsertRows(parent, position, position);
    parentItem->insertChild(position, item);
    endInsertRows();

    informDataChanged( parent );
}

bool ConnectDataModel::removeItem(const QModelIndex &index)
{
    ConnectDataItem* treeItem = getItem(index);
    if (treeItem) {
        if (!treeItem->isLastChild()) {
            if ((int)DataCheckState::KeyItem==treeItem->data((int)DataItemColumn::CheckState).toInt()) {
                QModelIndex sibling = index.sibling(index.row()+1, 0);
                int state = sibling.data((int)DataItemColumn::CheckState).toInt();
                if ((int)DataCheckState::ListAppend==state || (int)DataCheckState::MapAppend==state)
                    removeRows(index.row()+1, 1, parent(sibling));
            }
        }
        return removeRows(treeItem->row(), 1, parent(index));
    } else {
        return false;
    }
}

bool ConnectDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    ConnectDataItem* parentItem = getItem(parent);
    bool success = false;
    if (count > 0) {
        beginInsertRows(parent, row, row + count - 1);
        for (int i=0; i<count; ++i) {
            parentItem->insertChild(row+i, mRootItem);
        }
        endInsertRows();
        informDataChanged(parent);
        success = true;
    }
    return success;
}

bool ConnectDataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    ConnectDataItem* parentItem = getItem(parent);
    bool success = false;
    if (count > 0) {
        beginRemoveRows(parent, row, row + count - 1);
        success = parentItem->removeChildren(row, count);
        endRemoveRows();
        informDataChanged(parent);
//        success = true;
    }
    return success;
}

bool ConnectDataModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    ConnectDataItem* destParentItem = getItem(destinationParent);
    ConnectDataItem* sourceParentItem = getItem(sourceParent);
    if (destParentItem != sourceParentItem)
        return false;

    if (destinationChild > sourceRow) { // move down
         beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);
         for(int i=0; i<count; ++i) {
             sourceParentItem->moveChildren(sourceRow+i, destinationChild-1);
         }
         endMoveRows();
    } else { // move up
        beginMoveRows(sourceParent, sourceRow, sourceRow+count, destinationParent, destinationChild);
        for(int i=0; i<count; ++i) {
            sourceParentItem->moveChildren(sourceRow+i, destinationChild);
        }
        endMoveRows();
    }
    informDataChanged(destinationParent);

    return true;
}

QStringList ConnectDataModel::mimeTypes() const
{
    QStringList types;
    types <<  "application/vnd.gams-connect.text";
    return types;
}

Qt::DropActions ConnectDataModel::supportedDropActions() const
{
    return Qt::CopyAction ;
}

bool ConnectDataModel::canDropMimeData(const QMimeData *mimedata, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row);
    Q_UNUSED(parent)
    if (action != Qt::CopyAction)
        return false;

    if (!mimedata->hasFormat("application/vnd.gams-connect.text"))
        return false;

    QStringList newItems;
    QByteArray encodedData = mimedata->data("application/vnd.gams-connect.text");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    int rows = stream.atEnd()?-1:0;
    while (!stream.atEnd()) {
       QString text;
       stream >> text;
       newItems << text;
       ++rows;
    }
    if (column > 0) // not the first column
        return false;

    QStringList schemastrlist = newItems[0].split("=");
    qDebug() << schemastrlist << " :: " << "0 can drop ? :("  << row << "," << column << ") parent("  << parent.row() <<","<< parent.column() << ")" ;
    if (schemastrlist.size() <= 1)
        return false;

    if (!parent.isValid())
        return false;

    qDebug() << schemastrlist[1] << ", "
            << index(row, (int)DataItemColumn::SchemaKey, parent).data(Qt::DisplayRole).toString() << ", ";
    QStringList tobeinsertSchemaKey = schemastrlist[1].split(":");
    QStringList schemaKey = (row < 0 || column < 0 ? parent.sibling(parent.row(), (int)DataItemColumn::SchemaKey).data(Qt::DisplayRole).toString().split(":")
                                                         : index(row, (int)DataItemColumn::SchemaKey, parent).data(Qt::DisplayRole).toString().split(":")           );
    qDebug() << "tobeinsertSchemaKey=" << tobeinsertSchemaKey << " :: "
             << "schemaKey=" << schemaKey;

    int state = parent.sibling(parent.row(), (int)DataItemColumn::CheckState).data(Qt::DisplayRole).toInt();

    if (row < 0 || column < 0) { // drop on to a row
        if (state!=(int)DataCheckState::SchemaName) {  // drop on to a non-schemaname
            if (state!=(int)DataCheckState::ListItem) // not a ListItem
                return false;
            if (schemaKey.size()==tobeinsertSchemaKey.size()) // same size
                return false;
            if (tobeinsertSchemaKey.size()!=schemaKey.size()+1) // not immediate attribute to be inserted
                return false;
            if (existsUnderSameParent(schemastrlist[1],  parent)) // exists under the same parent
                return false;
        } else { // drop on to a schema name
            if (hasChildren(parent)) { // schema name has a child
                if (schemaKey.size()+1!=tobeinsertSchemaKey.size()) // not immediate attribute of schemaname
                    return false;
                if (!hasSameParent(tobeinsertSchemaKey, schemaKey,false)) // not immediate attribute of schemaname
                    return false;
                if (existsUnderSameParent(schemastrlist[1],  parent)) // attribute already exists
                    return false;
            } else {  // schema name without child
                if (schemaKey.size()==tobeinsertSchemaKey.size()) // has the same schema size
                    return false;
                if (hasSameParent(tobeinsertSchemaKey, schemaKey)!=0) // not same parent
                    return false;
                if (schemaKey.size()+1!=tobeinsertSchemaKey.size()) // not immediate attribute of schemaname
                    return false;
            }
        }
    } else { // drop between row
        if (schemaKey.size() > 0 && schemaKey.at(0).isEmpty())
            return false;

        if (!hasSameParent(tobeinsertSchemaKey, schemaKey,true)) // does not have the same parent
            return false;

        if (tobeinsertSchemaKey.size()>1) {  // when insert attribute
           // check if tobeinsertSchemaKey exists under the same parent
           if (tobeinsertSchemaKey.size()!=schemaKey.size() ||
               existsUnderSameParent(schemastrlist[1],  parent)) {
               return false;
           }
        }
    }

    if (schemaKey.at(0).isEmpty())
       return false;

    qDebug() << "00 can dropmimedata:";
     return true;
}

bool ConnectDataModel::dropMimeData(const QMimeData *mimedata, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    qDebug() << "0 dropmimedata:("  << row << "," << column << ")";
    if (action == Qt::IgnoreAction)
        return true;

    QByteArray encodedData = mimedata->data("application/vnd.gams-connect.text");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = stream.atEnd()?-1:0;
    while (!stream.atEnd()) {
       QString text;
       stream >> text;
       newItems << text;
       ++rows;
    }
    QStringList schemastrlist = newItems[0].split("=");
    if (!parent.isValid()) {
        return false;
    } else {

        qDebug() << "12 dropmimedata:("  << row << "," << column << ") ";
        qDebug() << "          parent(" << parent.row()<< "," << parent.column() << ")";
        int insertRow = -1;
        if (row >=0 && row < rowCount(parent)) {
            qDebug() << "121";
            insertRow = row;
        } else { // append,  drop at the last row
            qDebug() << "122";
            insertRow = row-1;
        }
        QStringList tobeinsertSchemaKey = schemastrlist[1].split(":");
        QStringList insertSchemaKey = (row < 0 || column < 0 ? parent.sibling(insertRow, (int)DataItemColumn::SchemaKey).data(Qt::DisplayRole).toString().split(":")
                                                             : index(insertRow, (int)DataItemColumn::SchemaKey, parent).data(Qt::DisplayRole).toString().split(":")           );

        QString schemaname = tobeinsertSchemaKey.first();
        if (tobeinsertSchemaKey.size() == 1) {
            qDebug() << "tobeinsertSchemaKey.size() == 1 :: " << tobeinsertSchemaKey;
            emit fromSchemaInserted(schemaname, row);
            qDebug() << "3 dropmimedata";
        } else {
            bool onlyRequired = mOnlyRequriedAttributesAdded;
            if (tobeinsertSchemaKey.size() == 1 && row < 0)  // drop onto schemaname
                onlyRequired = false;
            qDebug() << " drop onto schemaname ? " << (onlyRequired?"YES":"NO");
            ConnectData* data = mConnect->createDataHolderFromSchema(tobeinsertSchemaKey, onlyRequired );
            qDebug() << "data=" << data->str().c_str();
            if (data->getRootNode().IsNull())
                return false;
            tobeinsertSchemaKey.removeFirst();
            tobeinsertSchemaKey.removeLast();
            appendMapElement(schemaname, tobeinsertSchemaKey, data,  row, parent);
            emit modificationChanged(true);
            if (row < 0)
                emit indexExpandedAndResized(index(parent.row(), (int)DataItemColumn::Key, parent));
            else
                emit indexExpandedAndResized(index(row, (int)DataItemColumn::Key, parent));
            qDebug() << "4 dropmimedata";
        }
        return true;
    }

    return false;
}

void ConnectDataModel::addFromSchema(const QString& schemaname, int position)
{
    QStringList strlist;
    strlist << schemaname;
    ConnectData* data = mConnect->createDataHolder( strlist, mOnlyRequriedAttributesAdded );
    qDebug() << data->str().c_str();
    qDebug() << "position=" << position  << ", rowCount()=" << rowCount()
             << ", schemaCount()=" << rowCount(index(0,0));

    Q_ASSERT(data->getRootNode().Type() ==YAML::NodeType::Sequence);

    YAML::Node node = data->getRootNode();
    Q_ASSERT(node.Type()==YAML::NodeType::Sequence);

    QList<ConnectDataItem*> parents;
    parents << mRootItem << mRootItem->child(0);

    beginInsertRows(indexForTreeItem(parents.last()), position, position+1);
    for(size_t i = 0; i<node.size(); i++) {
        for (YAML::const_iterator it = node[i].begin(); it != node[i].end(); ++it) {
            QString schemaName = QString::fromStdString(it->first.as<std::string>());
            QStringList dataKeys;
            QList<QVariant> listData;
            listData << schemaName;
            listData << "";
            listData << QVariant((int)DataCheckState::SchemaName);
            listData << QVariant(QStringList());
            listData << QVariant(QStringList());
            listData << QVariant(true); // A0
            listData << ""; // A2
            listData << ""; // A3
            listData << ""; // A4
            listData << QVariant(QStringList(schemaName));
            listData << (mConnect->getSchema(schemaName) ? QVariant(false):QVariant(true));
            if (position>=parents.last()->childCount() || position < 0) {
                parents.last()->appendChild(new ConnectDataItem(listData, mItemIDCount++, parents.last()));
                parents << parents.last()->child(parents.last()->childCount()-1);
            } else {
                parents.last()->insertChild(position, new ConnectDataItem(listData, mItemIDCount++, parents.last()));
                parents << parents.last()->child(position);
            }

            insertSchemaData(schemaName, dataKeys, new ConnectData(it->second), -1,  parents);

            parents.pop_back();
        }
        position++;
    }

    endInsertRows();

    informDataChanged( index(0,0).parent() );
    QModelIndex parent = indexForTreeItem( mRootItem->child(0) );
    emit indexExpandedAndResized(index(position-1, (int)DataItemColumn::Key, parent));
}

void ConnectDataModel::appendMapElement(const QModelIndex &index)
{
    QList<QVariant> mapSeqData;
    mapSeqData << "[key]";
    mapSeqData << "[value]";
    mapSeqData << QVariant((int)DataCheckState::ElementMap);
    mapSeqData << QVariant();
    mapSeqData << QVariant();
    mapSeqData << QVariant(true);
    mapSeqData << QVariant();
    mapSeqData << QVariant();
    mapSeqData << QVariant();
    mapSeqData << QVariant(QStringList());
    mapSeqData << QVariant(false);
    ConnectDataItem *item = new ConnectDataItem( mapSeqData, mItemIDCount++, getItem(index.parent()) );
    insertItem(index.row(), item, index.parent());
}

void ConnectDataModel::appendMapElement(const QString& schemaname, QStringList& keys, ConnectData* data, int position, const QModelIndex& parentIndex)
{
    ConnectDataItem* parentItem = getItem(parentIndex);

    qDebug() << "appendMapElement (" << position << ") " << parentItem->childCount();
    QList<ConnectDataItem*> parents;
    parents << parentItem;

    QStringList schemaKeys;
    schemaKeys << schemaname << keys;

    YAML::Node node = data->getRootNode();
    beginInsertRows(parentIndex, position, position-1);
    insertSchemaData(schemaname, keys, data, position, parents);
    endInsertRows();

    informDataChanged(parentIndex);
    qDebug() << "end appendMapElement (" << position << ") " << rowCount();
}

void ConnectDataModel::appendListElement(const QString& schemaname,  QStringList& keys, ConnectData* data, const QModelIndex &index)
{
    QModelIndex parentIndex = index.parent();
    ConnectDataItem* parentItem = getItem(parentIndex);
    qDebug() << "appendListElement (" << index.row() << "," << index.column() << ") " << parentItem->childCount();

    QList<ConnectDataItem*> parents;
    parents << parentItem;

    QStringList schemaKeys;
    schemaKeys << schemaname << keys;

    beginInsertRows(parentIndex, index.row(), index.row()-1);

    ConnectSchema* schema = mConnect->getSchema(schemaname);
    YAML::Node node = data->getRootNode();
    for(size_t i = 0; i<node.size(); i++) {
        QList<QVariant> mapSeqData;
        QString listkey = schemaKeys.last();
        mapSeqData << QVariant::fromValue(index.row()+i);
        mapSeqData << "";
        mapSeqData << QVariant((int)DataCheckState::ListItem);
        mapSeqData << QVariant(QStringList());
        mapSeqData << QVariant(QStringList());
        mapSeqData << QVariant(true);
        mapSeqData << QVariant();
        mapSeqData << QVariant();
        mapSeqData << QVariant();
        schemaKeys << "-";
        keys       << "-";
        mapSeqData << QVariant(schemaKeys);
        mapSeqData << (schema ?  QVariant(false) : QVariant(true));
        ConnectDataItem* item = new ConnectDataItem( mapSeqData, mItemIDCount++, getItem(index.parent()));
        parents.last()->insertChild(index.row(), item );
        parents << item;

        if (node[i].Type()==YAML::NodeType::Map) {
            qDebug() << " .... 1 " << keys;
           insertSchemaData(schemaname, keys, new ConnectData(node[i]), -1, parents);
        } else if (node[i].Type()==YAML::NodeType::Scalar) {
            qDebug() << " .... 2 " << keys;
//            schema = mConnect->getSchema(schemaname);
            QString key = QString::fromStdString(node[i].as<std::string>());
            QList<QVariant> itemData;
            itemData << key;
            itemData << "";
            itemData << QVariant((int)DataCheckState::ElementKey);
            itemData << QVariant(schema->getTypeAsStringList(keys.join(":")));
            itemData << QVariant(schema->getAllowedValueAsStringList(keys.join(":")));
            itemData << QVariant(false);
            itemData << QVariant(false);
            itemData << QVariant();
            itemData << QVariant();
            itemData << QVariant();
            itemData << QVariant(false);
            parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
        }
    }
    endInsertRows();

    informDataChanged(parentIndex);
    qDebug() << "end appendListElement (" << index.row() << "," << index.column() << ") " << rowCount();
}

void ConnectDataModel::onlyRequriedAttributedChanged(int state)
{
     mOnlyRequriedAttributesAdded = (state==Qt::Checked);
}

void ConnectDataModel::reloadConnectDataModel()
{
    try {
        ConnectData* data = mConnect->loadDataFromFile(mLocation);
        delete mConnectData;
        mConnectData = data;
    } catch (std::exception &e) {
        EXCEPT() << e.what();
    }

    beginResetModel();
    if (mRootItem)
        delete mRootItem;

    mItemIDCount = 0;

    setupTreeItemModelData();
    endResetModel();
}

ConnectData *ConnectDataModel::getConnectData()
{
     YAML::Node root = YAML::Node(YAML::NodeType::Sequence);
     Q_ASSERT(mRootItem->childCount()==1);
     for(int i=0; i<mRootItem->childCount(); ++i) {
         ConnectDataItem* rootitem = mRootItem->child(i);
         for(int j=0; j<rootitem->childCount(); ++j) {
            ConnectDataItem* item = rootitem->child(j);
            YAML::Node node = YAML::Node(YAML::NodeType::Map);
            std::string key = item->data((int)DataItemColumn::Key).toString().toStdString();
            YAML::Node mapnode = YAML::Node(YAML::NodeType::Map);
            getData( item, mapnode );
            node[key] = mapnode;
            root[j] = node;
         }
     }
     return new ConnectData(root);
}

bool ConnectDataModel::hasSameParent(const QStringList& tobeinsertSchema, const QStringList& schemaKey, bool samelevel) const
{
    if (samelevel) {
        if (tobeinsertSchema.size()!=schemaKey.size())
            return false;
        for(int i = 0; i<schemaKey.size()-1; ++i) {
            qDebug() << "  1 check same parent ..." << tobeinsertSchema.at(i);
            if (tobeinsertSchema.at(i).compare(schemaKey.at(i), Qt::CaseSensitive)!=0) {
                qDebug() << " 11  check same parent ...";
                return false;
            }
        }
    } else {
        if (tobeinsertSchema.size()<schemaKey.size())
            return false;
        for(int i = 0; i<schemaKey.size(); ++i) {
            qDebug() << "  2 check same parent ..." << tobeinsertSchema.at(i);
            if (tobeinsertSchema.at(i).compare(schemaKey.at(i), Qt::CaseSensitive)!=0) {
                qDebug() << " 21  check same parent ...";
                return false;
            }
        }
    }
    return true;
}

bool ConnectDataModel::existsUnderSameParent(const QString& tobeinsertSchema, const QModelIndex &parent, bool samelevel) const
{
    int level = tobeinsertSchema.split(":").size();
    qDebug() << tobeinsertSchema << " exists ? under" << parent.sibling(parent.row(), (int)DataItemColumn::SchemaKey).data(Qt::DisplayRole).toString();
    for (int i =0; i<rowCount(parent); ++i) {
        QString schemaKey = (index(i, (int)DataItemColumn::SchemaKey,parent).data(Qt::DisplayRole).toString());
        QString tobeinsertedname = (tobeinsertSchema.contains("[") ? tobeinsertSchema.left(tobeinsertSchema.lastIndexOf("["))
                                                                   : tobeinsertSchema);
        QString keyname= (schemaKey.contains("[") ? schemaKey.left(tobeinsertSchema.lastIndexOf("["))
                                                  : schemaKey);
        if (tobeinsertedname.compare(keyname)==0)
            return true;
        int keylevel = schemaKey.split(":").size();
        if (!samelevel && qAbs(level-keylevel) != 1 )
            return false;
    }
    return false;
}

void ConnectDataModel::getData(ConnectDataItem *item, YAML::Node& node)
{
    for(int i=0; i<item->childCount(); ++i) {
        ConnectDataItem* childitem = item->child(i);
        if (childitem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::SchemaName) {
            std::string key = childitem->data((int)DataItemColumn::Key).toString().toStdString();
            YAML::Node mapnode = YAML::Node(YAML::NodeType::Map);
            getData( childitem, mapnode );
            node[key] = mapnode;
        } else if (childitem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::KeyItem) {
            std::string key = childitem->data((int)DataItemColumn::Key).toString().toStdString();
            YAML::Node mapnode;
            getData( childitem, mapnode );
            node[key] = mapnode;
        } else if (childitem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ListItem) {
                   YAML::Node mapnode;
                   for(int j=0; j<childitem->childCount(); ++j) {  // skip row with DataCheckState::ListItem
                       ConnectDataItem* seqchilditem = childitem->child(j);
                       std::string seqmapkey = seqchilditem->data((int)DataItemColumn::Key).toString().toStdString();
                       if (seqchilditem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ElementValue ||
                           seqchilditem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ElementMap      ) {
                           YAML::Node mapseqnode;
                           getData(seqchilditem, mapseqnode);
                           mapnode[seqmapkey] = seqchilditem->data((int)DataItemColumn::Value).toString().toStdString();;
                       } else if (seqchilditem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ElementKey ) {
                           mapnode = seqmapkey;
                       } else {
                           YAML::Node mapseqnode;
                           getData(seqchilditem, mapseqnode);
                           mapnode[seqmapkey] = mapseqnode;
                       }
                   }
                   node[i] = mapnode;
        } else if (childitem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ElementValue ||
                   childitem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ElementMap      ) {
                   std::string key = childitem->data((int)DataItemColumn::Key).toString().toStdString();
                   node[key] = childitem->data((int)DataItemColumn::Value).toString().toStdString();
        }  else if (childitem->data((int)DataItemColumn::CheckState).toInt()==(int)DataCheckState::ElementKey) {
                  YAML::Node node = YAML::Node(YAML::NodeType::Scalar);
                  node = YAML::Node(childitem->data((int)DataItemColumn::Key).toString().toStdString());
        }
    }
}

void ConnectDataModel::informDataChanged(const QModelIndex& parent)
{
    QModelIndex checkstate_index = parent.sibling(parent.row(), (int)DataItemColumn::CheckState);
    int state = checkstate_index.data(Qt::DisplayRole).toInt();
    int childcount = getItem(parent)->childCount();
    if ((int)DataCheckState::KeyItem==state) {
        if (childcount <= 2) {
            for (int i=0; i<childcount; ++i) {
                ConnectDataItem* item = getItem(index(i, 0, parent));
                item->setData((int)DataItemColumn::MoveDown, QVariant( false ));
                item->setData((int)DataItemColumn::MoveUp, QVariant( false ));
                informDataChanged( index(i, 0, parent) );
            }
        } else { // childcount > 2
            int i = 0;
            while (i<childcount-1) {
                ConnectDataItem* item = getItem(index(i, 0, parent));
                if (item->data((int)DataItemColumn::CheckState).toUInt()==(int)DataCheckState::ListItem ||
                    item->data((int)DataItemColumn::CheckState).toUInt()==(int)DataCheckState::ElementMap    ) {
                    bool firstChild = (i==0 && i!=(childcount-2));
                    bool lastChild  = (i!=0 && i==(childcount-2));
                    item->setData((int)DataItemColumn::MoveDown, QVariant( !lastChild ));
                    item->setData((int)DataItemColumn::MoveUp, QVariant( !firstChild ));
                } else {
                    item->setData((int)DataItemColumn::MoveDown, QVariant( false ));
                    item->setData((int)DataItemColumn::MoveUp, QVariant( false ));
                }
                informDataChanged( index(i, 0, parent) );
                ++i;
            }
            ConnectDataItem* item = getItem(index(i, 0, parent));
            item->setData((int)DataItemColumn::MoveDown, QVariant( false ));
            item->setData((int)DataItemColumn::MoveUp, QVariant( false ));
        }
    } else {
        for (int i=0; i<childcount; ++i) {
            ConnectDataItem* item = getItem(index(i, 0, parent));
            if (item->data((int)DataItemColumn::CheckState).toUInt()==(int)DataCheckState::ListItem  ||
                item->data((int)DataItemColumn::CheckState).toUInt()==(int)DataCheckState::SchemaName   ) {
                item->setData((int)DataItemColumn::MoveDown, QVariant( !item->isLastChild()) );
                item->setData((int)DataItemColumn::MoveUp, QVariant( !item->isFirstChild()) );
            } else {
                item->setData((int)DataItemColumn::MoveDown, QVariant( false ));
                item->setData((int)DataItemColumn::MoveUp, QVariant( false ));
            }
            informDataChanged( index(i, 0, parent) );
        }
    }

    emit dataChanged(index(0, (int)DataItemColumn::MoveDown, parent),
                     index(getItem(parent)->childCount()-1, (int)DataItemColumn::ElementID, parent),
                     QVector<int> { Qt::DisplayRole, Qt::ToolTipRole, Qt::DecorationRole} );

}

void ConnectDataModel::setupTreeItemModelData()
{
    QList<QVariant> rootData;
    rootData << "Key"               << "Value"         << "" /*State*/    << "" /*Type*/
             << "" /*AllowedValue*/ << "" /*Required*/ << "" /*MoveDown*/ << "" /*MoveUp*/
             << "" /*ID*/           << "" /*Schema*/   << "" /*Unknown*/;

    mRootItem = new ConnectDataItem(rootData, mItemIDCount++);

    QList<ConnectDataItem*> parents;
    parents << mRootItem;

    QList<QVariant> rootListData;
    rootListData << QVariant();
    rootListData << QVariant();
    rootListData << QVariant((int)DataCheckState::Root);
    rootListData << QVariant();
    rootListData << QVariant();
    rootListData << QVariant();
    rootListData << QVariant();
    rootListData << QVariant();
    rootListData << QVariant();
    rootListData << QVariant();
    rootListData << QVariant();
    parents.last()->appendChild(new ConnectDataItem(rootListData, mItemIDCount++, parents.last()));
    parents << parents.last()->child(parents.last()->childCount()-1);

    if (!mConnectData->getRootNode().IsNull()) {
        YAML::Node node = mConnectData->getRootNode();
        Q_ASSERT(node.Type()==YAML::NodeType::Sequence);

        int position = 0;
        for(size_t i = 0; i<node.size(); i++) {
            for (YAML::const_iterator it = node[i].begin(); it != node[i].end(); ++it) {
                QString schemaName = QString::fromStdString(it->first.as<std::string>());
                ConnectSchema* schema = mConnect->getSchema(schemaName);
                QStringList dataKeys;
                QList<QVariant> listData;
                listData << schemaName;
                listData << "";
                listData << QVariant((int)DataCheckState::SchemaName);
                listData << QVariant(QStringList());
                listData << QVariant(QStringList());
                listData << QVariant(true);
                listData << QVariant();
                listData << QVariant();
                listData << QVariant();
                listData << QVariant(QStringList(schemaName));
                listData << (schema ? QVariant(false) : QVariant(true));
                if (position>=parents.last()->childCount() || position < 0) {
                    parents.last()->appendChild(new ConnectDataItem(listData, mItemIDCount++, parents.last()));
                    parents << parents.last()->child(parents.last()->childCount()-1);
                } else {
                    parents.last()->insertChild(position, new ConnectDataItem(listData, mItemIDCount++, parents.last()));
                    parents << parents.last()->child(position);
                }

                insertSchemaData(schemaName, dataKeys, new ConnectData(it->second), -1, parents);

                parents.pop_back();
            }
            position++;
        }
        informDataChanged( index(0,0).parent() );
    }
}

void ConnectDataModel::insertSchemaData(const QString& schemaName, const QStringList& keys, ConnectData* data, int position, QList<ConnectDataItem*>& parents)
{
    QStringList dataKeys(keys);
    ConnectSchema* schema = mConnect->getSchema(schemaName);

    QStringList schemaKeys;
    schemaKeys << schemaName;
    if (!dataKeys.isEmpty())
        schemaKeys << dataKeys;

    for (YAML::const_iterator mit = data->getRootNode().begin(); mit != data->getRootNode().end(); ++mit) {
         QString mapToSequenceKey = "";
         if (mit->second.Type()==YAML::NodeType::Scalar) {
             QString key = QString::fromStdString(mit->first.as<std::string>());
             dataKeys << key;
             schemaKeys << key;
             QList<QVariant> itemData;
             itemData << (key.contains("[") ? key.left(key.lastIndexOf("[")) : key);
             itemData << QVariant(mit->second.as<std::string>().c_str());
             itemData << QVariant((int)DataCheckState::ElementValue);
             itemData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))) : QVariant());
             itemData << (schema ? QVariant(schema->getAllowedValueAsStringList(dataKeys.join(":"))) : QVariant());
             itemData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))) : QVariant());
             itemData << QVariant();
             itemData << QVariant();
             itemData << QVariant();
             itemData << QVariant(schemaKeys);
             itemData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                 : QVariant(true));
             if (position>=parents.last()->childCount() || position < 0) {
                 parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
             } else {
                 parents.last()->insertChild(position, new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
             }

             dataKeys.removeLast();
             schemaKeys.removeLast();
         } else if (mit->second.Type()==YAML::NodeType::Map) {
                   QString key = QString::fromStdString(mit->first.as<std::string>());
                   dataKeys << key;
                   schemaKeys << key;
                   QList<QVariant> itemData;
                   itemData << (key.contains("[") ? key.left(key.lastIndexOf("[")) : key);
                   itemData << ""; // TODO
                   itemData << QVariant((int)DataCheckState::KeyItem);
                   itemData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))) : QVariant());
                   itemData << QVariant(QStringList());
                   itemData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))) : QVariant());
                   itemData << QVariant();
                   itemData << QVariant();
                   itemData << QVariant();
                   itemData << QVariant(schemaKeys);
                   itemData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                       : QVariant(true));
                   if (position>=parents.last()->childCount() || position < 0) {
                       parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
                       parents << parents.last()->child(parents.last()->childCount()-1);
                   } else {
                       parents.last()->insertChild(position, new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
                       parents << parents.last()->child(position);
                   }
                   int k = 0;
                   for (YAML::const_iterator dmit = mit->second.begin(); dmit != mit->second.end(); ++dmit) {
                       QString mapkey = QString::fromStdString(dmit->first.as<std::string>());
//                       dataKeys << mapkey;
                       if (dmit->second.Type()==YAML::NodeType::Scalar) {
                           QList<QVariant> mapitemData;
                           mapitemData << mapkey;
                           mapitemData << QVariant(dmit->second.as<std::string>().c_str());
                           mapitemData << QVariant((int)DataCheckState::ElementMap);
                           mapitemData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))) : QVariant());
                           mapitemData << QVariant(QStringList());
                           mapitemData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))) : QVariant());
                           mapitemData << QVariant();
                           mapitemData << QVariant();
                           mapitemData << QVariant();
                           mapitemData << QVariant(QStringList());
                           mapitemData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                                  : QVariant(true));
                           parents.last()->appendChild(new ConnectDataItem(mapitemData, mItemIDCount++, parents.last()));
                           k++;
                       } else if (dmit->second.Type()==YAML::NodeType::Null) {
                                  QList<QVariant> mapitemData;
                                  mapitemData << mapkey;
                                  mapitemData << "null";
                                  mapitemData << QVariant((int)DataCheckState::ElementMap);
                                  mapitemData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))) : QVariant());
                                  mapitemData << QVariant(QStringList());
                                  mapitemData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))) : QVariant());
                                  mapitemData << QVariant();
                                  mapitemData << QVariant();
                                  mapitemData << QVariant();
                                  mapitemData << QVariant(QStringList());
                                  mapitemData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                                         : QVariant(true));
                                  parents.last()->appendChild(new ConnectDataItem(mapitemData, mItemIDCount++, parents.last()));
                                  k++;
                       } else {
                           Q_ASSERT(dmit->second.Type()==YAML::NodeType::Scalar || dmit->second.Type()==YAML::NodeType::Null);
                       }
//                       dataKeys.removeLast();
                   }

                   QList<QVariant> sequenceDummyData;
                   sequenceDummyData << (key.contains("[") ? key.left(key.lastIndexOf("[")) : key);
                   sequenceDummyData << "";
                   sequenceDummyData << QVariant((int)DataCheckState::MapAppend);
                   sequenceDummyData << QVariant(QStringList());
                   sequenceDummyData << QVariant(QStringList());
                   sequenceDummyData << QVariant(false);
                   sequenceDummyData << QVariant();
                   sequenceDummyData << QVariant();
                   sequenceDummyData << QVariant();
                   sequenceDummyData << QVariant(QStringList());
                   sequenceDummyData << (schema ? QVariant(false) : QVariant(true));
                   sequenceDummyData << QVariant(true);
                   parents.last()->appendChild(new ConnectDataItem(sequenceDummyData, mItemIDCount++, parents.last()));

                   parents.pop_back();
                   dataKeys.removeLast();
                   schemaKeys.removeLast();
         } else if (mit->second.Type()==YAML::NodeType::Sequence) {
             QString key = QString::fromStdString(mit->first.as<std::string>());
             bool isAnyofDefined = schema ? schema->isAnyOfDefined(key) : false;
             if (isAnyofDefined) {
                 QStringList anyofSchemaLlist = schema ? schema->getAllAnyOfKeys(key) : QStringList();
                 key += "[0]";
             }
             mapToSequenceKey = key;
             dataKeys   << key;
             schemaKeys << key;
             QList<QVariant> itemData;
             itemData << (key.contains("[") ? key.left(key.lastIndexOf("[")) : key);
             itemData << "";
             itemData << QVariant((int)DataCheckState::KeyItem);
             itemData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))) : QVariant());
             itemData << QVariant(QStringList());
             itemData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))) : QVariant());
             itemData << QVariant();
             itemData << QVariant();
             itemData << QVariant();
             itemData << QVariant(schemaKeys);
             itemData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                 : QVariant(true));
             if (position>=parents.last()->childCount() || position < 0) {
                 parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
                 parents << parents.last()->child(parents.last()->childCount()-1);
             } else {
                 parents.last()->insertChild(position, new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
                 parents << parents.last()->child(position);
             }
             dataKeys   << "-";
             schemaKeys << "-";
             for(size_t k = 0; k<mit->second.size(); k++) {
                 QList<QVariant> indexData;
                 indexData << QVariant::fromValue(k);
                 indexData << QVariant(QStringList());
                 indexData << QVariant((int)DataCheckState::ListItem);
                 indexData << QVariant(QStringList());
                 indexData << QVariant(QStringList());
                 QStringList Keys(dataKeys);
                 if (Keys.last().compare("-")==0)
                     Keys.removeLast();
                 indexData << QVariant(true);
                 indexData << QVariant();
                 indexData << QVariant();
                 indexData << QVariant();
                 indexData << QVariant(schemaKeys);
                 indexData << (schema ? (schema->contains(Keys.join(":")) ? QVariant(false) : QVariant(true))
                                      : QVariant(true));
                 parents.last()->appendChild(new ConnectDataItem(indexData, mItemIDCount++, parents.last()));

                        if (mit->second[k].Type()==YAML::NodeType::Map) {
                           parents << parents.last()->child(parents.last()->childCount()-1);
                           const YAML::Node mapnode = mit->second[k];
                           for (YAML::const_iterator mmit = mapnode.begin(); mmit != mapnode.end(); ++mmit) {
                               key =  QString::fromStdString( mmit->first.as<std::string>() );
                               dataKeys   << key;
                               schemaKeys << key;
                               if (mmit->second.Type()==YAML::NodeType::Sequence) {
                                   QList<QVariant> seqSeqData;
                                   seqSeqData << key;
                                   seqSeqData << "";
                                   seqSeqData << QVariant((int)DataCheckState::KeyItem);
                                   seqSeqData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))) : QVariant());
                                   seqSeqData << QVariant(QStringList());
                                   seqSeqData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))) : QVariant());
                                   seqSeqData << QVariant();
                                   seqSeqData << QVariant();
                                   seqSeqData << QVariant();
                                   seqSeqData << QVariant(schemaKeys);
                                   seqSeqData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                                         : QVariant(true));
                                   parents.last()->appendChild(new ConnectDataItem(seqSeqData, mItemIDCount++, parents.last()));

                                   dataKeys   << "-";
                                   schemaKeys << "-";
                                   parents << parents.last()->child(parents.last()->childCount()-1);
                                   for(size_t kk = 0; kk<mmit->second.size(); kk++) {
                                       QList<QVariant> indexSeqData;
                                       indexSeqData << QVariant::fromValue(kk);
                                       indexSeqData << QVariant(QStringList());
                                       indexSeqData << QVariant((int)DataCheckState::ListItem);
                                       indexSeqData << QVariant(QStringList());
                                       indexSeqData << QVariant(QStringList());
                                       QStringList Keys(dataKeys);
                                       if (Keys.last().compare("-")==0)
                                           Keys.removeLast();
                                       indexSeqData << QVariant(true);
                                       indexSeqData << QVariant();
                                       indexSeqData << QVariant();
                                       indexSeqData << QVariant();
                                       indexSeqData << QVariant(schemaKeys);
                                       indexSeqData << (schema ? (schema->contains(Keys.join(":")) ? QVariant(false) : QVariant(true))
                                                               : QVariant(true));
                                       parents.last()->appendChild(new ConnectDataItem(indexSeqData, mItemIDCount++, parents.last()));

                                       if (mmit->second[kk].Type()==YAML::NodeType::Scalar) {
                                           parents << parents.last()->child(parents.last()->childCount()-1);
                                            QList<QVariant> indexScalarData;
                                           indexScalarData << mmit->second[kk].as<std::string>().c_str();
                                           indexScalarData << ""; // TODO
                                           indexScalarData << QVariant((int)DataCheckState::ElementKey);
                                           indexScalarData << QVariant(QStringList());
                                           indexScalarData << QVariant(QStringList());
                                           indexScalarData << QVariant();
                                           indexScalarData << QVariant();
                                           indexScalarData << QVariant();
                                           indexScalarData << QVariant();
                                           indexScalarData << QVariant(QStringList());
                                           indexScalarData << (schema ? QVariant(false) : QVariant(true));
                                           parents.last()->appendChild(new ConnectDataItem(indexScalarData, mItemIDCount++, parents.last()));
                                           parents.pop_back();
                                        } // TODO: else

                                        QList<QVariant> indexSeqDummyData;
                                        indexSeqDummyData << key;
                                        indexSeqDummyData << "";
                                        indexSeqDummyData << QVariant((int)DataCheckState::ListAppend);
                                        QStringList keys(dataKeys);
                                        keys.insert(0,schemaName);
                                        indexSeqDummyData << QVariant(QStringList());
                                        indexSeqDummyData << keys;
                                        indexSeqDummyData << QVariant();
                                        indexSeqDummyData << QVariant();
                                        indexSeqDummyData << QVariant();
                                        indexSeqDummyData << QVariant();
                                        indexSeqDummyData << QVariant(QStringList());
                                        indexSeqDummyData << (schema ? QVariant(false) : QVariant(true));
                                        parents.last()->appendChild(new ConnectDataItem(indexSeqDummyData, mItemIDCount++, parents.last()));

                                   }
                                   parents.pop_back();
                                   dataKeys.removeLast();
                                   schemaKeys.removeLast();

                               } else if (mmit->second.Type()==YAML::NodeType::Map) {
                                          QList<QVariant> mapData;
                                          mapData << key;
                                          mapData << "";
                                          mapData << QVariant((int)DataCheckState::KeyItem);
                                          mapData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))) : QVariant());
                                          mapData << QVariant(QStringList());
                                          mapData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))) : QVariant());
                                          mapData << QVariant();
                                          mapData << QVariant();
                                          mapData << QVariant();
                                          mapData << QVariant(schemaKeys);
                                          mapData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                                             : QVariant(true));
                                          parents.last()->appendChild(new ConnectDataItem(mapData, mItemIDCount++, parents.last()));

                                          parents << parents.last()->child(parents.last()->childCount()-1);
                                          const YAML::Node mapmapnode = mmit->second;
                                          for (YAML::const_iterator mmmit = mapmapnode.begin(); mmmit != mapmapnode.end(); ++mmmit) {
                                               QList<QVariant> mapSeqData;
                                               mapSeqData << mmmit->first.as<std::string>().c_str();
                                               mapSeqData << mmmit->second.as<std::string>().c_str();  // can be int/bool/double
                                               mapSeqData << QVariant((int)DataCheckState::ElementMap);
                                               mapSeqData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))): QVariant());
                                               mapSeqData << (schema ? QVariant(QVariant(schema->getAllowedValueAsStringList(dataKeys.join(":")))): QVariant());
                                               mapSeqData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))): QVariant());
                                               mapSeqData << QVariant();
                                               mapSeqData << QVariant();
                                               mapSeqData << QVariant();
                                               mapSeqData << QVariant(QStringList());
                                               mapSeqData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                                                     : QVariant(true));
                                               parents.last()->appendChild(new ConnectDataItem(mapSeqData, mItemIDCount++, parents.last()));
//                                                       dataKeys.removeLast();
                                          }
                                          QList<QVariant> indexSeqDummyData;
                                          indexSeqDummyData << key;
                                          indexSeqDummyData << "";
                                          indexSeqDummyData << QVariant((int)DataCheckState::MapAppend);
                                          indexSeqDummyData << QVariant(QStringList());
                                          indexSeqDummyData << QVariant(QStringList());
                                          indexSeqDummyData << QVariant();
                                          indexSeqDummyData << QVariant();
                                          indexSeqDummyData << QVariant();
                                          indexSeqDummyData << QVariant();
                                          indexSeqDummyData << QVariant(QStringList());
                                          indexSeqDummyData << (schema ? QVariant(false) : QVariant(true));
                                          parents.last()->appendChild(new ConnectDataItem(indexSeqDummyData, mItemIDCount++, parents.last()));

                                          parents.pop_back();

                               } else if (mmit->second.Type()==YAML::NodeType::Scalar) {
                                     QList<QVariant> mapSeqData;
                                     mapSeqData << key;
                                     mapSeqData << mmit->second.as<std::string>().c_str();
                                     mapSeqData << QVariant((int)DataCheckState::ElementValue);
                                     mapSeqData << (schema ? QVariant(schema->getTypeAsStringList(dataKeys.join(":"))): QVariant());
                                     mapSeqData << (schema ? QVariant(QVariant(schema->getAllowedValueAsStringList(dataKeys.join(":")))): QVariant());
                                     mapSeqData << (schema ? QVariant(!schema->isRequired(dataKeys.join(":"))): QVariant());
                                     mapSeqData << QVariant();
                                     mapSeqData << QVariant();
                                     mapSeqData << QVariant();
                                     mapSeqData << QVariant(schemaKeys);
                                     mapSeqData << (schema ? (schema->contains(dataKeys.join(":")) ? QVariant(false) : QVariant(true))
                                                           : QVariant(true));
                                     parents.last()->appendChild(new ConnectDataItem(mapSeqData, mItemIDCount++, parents.last()));
                               }
                               dataKeys.removeLast();
                               schemaKeys.removeLast();
                           }
                           parents.pop_back();
                        } else if (mit->second[k].Type()==YAML::NodeType::Scalar) {
                                   parents << parents.last()->child(parents.last()->childCount()-1);
                                   QList<QVariant> mapSeqData;
                                   mapSeqData << QVariant( QString::fromStdString(mit->second[k].as<std::string>()) );
                                   mapSeqData << "";
                                   mapSeqData << QVariant((int)DataCheckState::ElementKey);
                                   mapSeqData << QVariant(QStringList());
                                   mapSeqData << QVariant(QStringList());
                                   mapSeqData << QVariant();
                                   mapSeqData << QVariant();
                                   mapSeqData << QVariant();
                                   mapSeqData << QVariant();
                                   mapSeqData << QVariant(QStringList());
                                   mapSeqData << (schema ? QVariant(false) : QVariant(true));
                                   parents.last()->appendChild(new ConnectDataItem(mapSeqData, mItemIDCount++, parents.last()));
                                   parents.pop_back();
                        }
                    }
                    dataKeys.removeLast();
                    schemaKeys.removeLast();

                    QList<QVariant> sequenceDummyData;
                    sequenceDummyData << mapToSequenceKey;
                    sequenceDummyData << "";
                    sequenceDummyData << QVariant((int)DataCheckState::ListAppend);
                    QStringList keys(dataKeys);
                    keys.insert(0,schemaName);
                    sequenceDummyData << QVariant(QStringList());
                    sequenceDummyData << keys;
                    sequenceDummyData << QVariant();
                    sequenceDummyData << QVariant();
                    sequenceDummyData << QVariant();
                    sequenceDummyData << QVariant();
                    sequenceDummyData << QVariant(QStringList());
                    sequenceDummyData << (schema ? QVariant(false) : QVariant(true));
                    parents.last()->appendChild(new ConnectDataItem(sequenceDummyData, mItemIDCount++, parents.last()));

                    parents.pop_back();
                    dataKeys.removeLast();
                    schemaKeys.removeLast();
             }
    }
}

} // namespace connect
} // namespace studio
} // namespace gams
