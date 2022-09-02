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

#include "connectdatamodel.h"
#include "theme.h"
namespace gams {
namespace studio {
namespace connect {

ConnectDataModel::ConnectDataModel(const QString& filename,  Connect* c, QObject *parent)
    : QAbstractItemModel{parent},
      mItemIDCount(0),
      mLocation(filename),
      mConnect(c)
{
    mConnectData = mConnect->loadDataFromFile(mLocation);
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
//           return  QVariant(item->data(index.column()).toStringList().join(","));
        } else if (index.column()==(int)DataItemColumn::Key) {
                  QModelIndex checkstate_index = index.sibling(index.row(),(int)DataItemColumn::CheckState );
                  if (checkstate_index.data(Qt::DisplayRole).toInt()==(int)DataCheckState::ListItem) {
                     return QVariant(index.row());
                  } else {
                      return item->data(index.column());
                  }
        } else if (index.column()==(int)DataItemColumn::Expand) {
                 return QVariant(item->id());
        } else {
            return item->data(index.column());
        }
    }
    case Qt::ForegroundRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        int state = item->data( (int)DataItemColumn::CheckState ).toInt();
        if (state==(int)DataCheckState::Root)
            return  QVariant::fromValue(Theme::color(Theme::Syntax_declaration));
        else if (state==(int)DataCheckState::SchemaName)
                return  QVariant::fromValue(Theme::color(Theme::Syntax_embedded));
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
        if (parentItem == mRootItem) {
            return QVariant::fromValue(QApplication::palette().color(QPalette::Window));
        } else {
            if (item->data( (int)DataItemColumn::CheckState ).toInt() <= (int)DataCheckState::ListItem )
                return QVariant::fromValue(QGuiApplication::palette().color(QPalette::Window));
            else if (item->data( (int)DataItemColumn::CheckState ).toInt() == (int)DataCheckState::ListAppend ||
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
            if (parentItem == mRootItem) {
                if (index.column()==(int)DataItemColumn::Delete) {
                    return QVariant( QString("delete \"%1\" data including all children").arg( data_index.data(Qt::DisplayRole).toString()) );
                } else if (index.column()==(int)DataItemColumn::MoveUp) {
                          return QVariant( QString("move \"%1\" data up including all children").arg( data_index.data(Qt::DisplayRole).toString()) );
                } else if (index.column()==(int)DataItemColumn::MoveDown) {
                           return QVariant( QString("move \"%1\" data down including all it children").arg( data_index.data(Qt::DisplayRole).toString()) );
                } else if (index.column()==(int)DataItemColumn::Key) {
                          if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::SchemaName) {
                             return QVariant( QString("show help for \"%1\" schema").arg( data_index.data(Qt::DisplayRole).toString()) );
                          } else if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ListAppend) {
                                    return QVariant("add schmea element");
                          }
                }
            } else {
                if (index.column()==(int)DataItemColumn::Delete) {
                    return QVariant( QString("delete \"%1\" and all children if there is any").arg( data_index.data(Qt::DisplayRole).toString()) );
                } else if (index.column()==(int)DataItemColumn::MoveDown) {
                          if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ListItem &&
                              item->data(0).toInt() < parentItem->childCount()-1)
                              return QVariant( QString("move \"%1\" and all it children down").arg( data_index.data(Qt::DisplayRole).toString()) );
                } else if (index.column()==(int)DataItemColumn::MoveUp) {
                          if (item->data(0).toInt() > 0 && item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ListItem)
                              return QVariant( QString("move \"%1\" and all it children up").arg( data_index.data(Qt::DisplayRole).toString()) );
                } else  if (index.column()==(int)DataItemColumn::Key) {
                           QVariant data = index.data(index.column());
                           if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ListAppend) {
                               return QVariant( QString("add element to the list of %1").arg( data.toString() ) );
                           } else if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::MapAppend) {
                                      return QVariant( QString("add element to %1 dict").arg( data.toString() ) );
                           }
                }
            }
        }
        break;
    }
    case Qt::DecorationRole: {
        ConnectDataItem* item = static_cast<ConnectDataItem*>(index.internalPointer());
        QModelIndex checkstate_index = index.sibling(index.row(),(int)DataItemColumn::CheckState );
       if (index.column()==(int)DataItemColumn::Key) {
           if (checkstate_index.data(Qt::DisplayRole).toInt()==(int)DataCheckState::SchemaName) {
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
                   return Qt::NoItemFlags;
    } else if (index.column()==(int)DataItemColumn::Value) {
              if (item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ElementKey   ||
                  item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ElementValue ||
                  item->data( (int)DataItemColumn::CheckState ).toInt()==(int)DataCheckState::ElementMap      )
                  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
              else if (item->data( (int)DataItemColumn::CheckState ).toInt()<=(int)DataCheckState::ElementKey ||
                       item->data( (int)DataItemColumn::CheckState ).toInt()>=(int)DataCheckState::ListAppend)
                     return  Qt::NoItemFlags;
              else
                    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    } else if (index.column()>(int)DataItemColumn::CheckState) {
               return  Qt::NoItemFlags;
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
            qDebug() << "not last child";
            if ((int)DataCheckState::KeyItem==treeItem->data((int)DataItemColumn::CheckState).toInt()) {
                qDebug() << "     state="<< treeItem->data((int)DataItemColumn::CheckState).toInt();
                QModelIndex sibling = index.sibling(index.row()+1, 0);
                int state = sibling.data((int)DataItemColumn::CheckState).toInt();
                qDebug() << "     sibling state="<< state;
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
        success = true;
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

void ConnectDataModel::addFromSchema(ConnectData* data, int position)
{
    qDebug() << data->str().c_str();

    Q_ASSERT(data->getRootNode().Type() ==YAML::NodeType::Sequence);
    Q_ASSERT(position <= rowCount());

    YAML::Node node = data->getRootNode();
    Q_ASSERT(node.Type()==YAML::NodeType::Sequence);

    QList<ConnectDataItem*> parents;
    parents << mRootItem;

    beginInsertRows(QModelIndex(), position, position+1);
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
            if (position>=parents.last()->childCount()) {
                parents.last()->appendChild(new ConnectDataItem(listData, mItemIDCount++, parents.last()));
            } else {
                parents.last()->insertChild(position, new ConnectDataItem(listData, mItemIDCount++, parents.last()));
            }

            parents << parents.last()->child(parents.last()->childCount()-1);

            insertSchemaData(schemaName, dataKeys, new ConnectData(it->second), parents);

            parents.pop_back();
        }
        position++;
    }

    endInsertRows();

    informDataChanged( index(0,0).parent() );
}

void ConnectDataModel::appendMapElement(const QModelIndex &index)
{
    QList<QVariant> mapSeqData;
    mapSeqData << "[key]";
    mapSeqData << "[value]";
    mapSeqData << QVariant((int)DataCheckState::ElementMap);
    mapSeqData << "";
    mapSeqData << "";
    mapSeqData << QVariant(true);  // A0
    mapSeqData << "";  // A2
    mapSeqData << "";  // A3
    mapSeqData << "";  // A4
    ConnectDataItem *item = new ConnectDataItem( mapSeqData, mItemIDCount++, getItem(index.parent()) );
    insertItem(index.row(), item, index.parent());
}

void ConnectDataModel::appendListElement(const QString& schemaname,  QStringList& keys, ConnectData* data, const QModelIndex &index)
{
    QModelIndex parentIndex = index.parent();
    ConnectDataItem* parentItem = getItem(parentIndex);
    qDebug() << "appendListElement (" << index.row() << "," << index.column() << ") " << parentItem->childCount();

    QList<ConnectDataItem*> parents;
    parents << parentItem;

    beginInsertRows(parentIndex, index.row(), index.row()-1);

    YAML::Node node = data->getRootNode();
    for(size_t i = 0; i<node.size(); i++) {
        QList<QVariant> mapSeqData;
        mapSeqData << QVariant::fromValue(index.row()+i);
        mapSeqData << "";
        mapSeqData << QVariant((int)DataCheckState::ListItem);
        mapSeqData << QVariant(QStringList());
        mapSeqData << QVariant(QStringList());
        mapSeqData << QVariant(true);  // A0
        mapSeqData << QVariant(false);  // A2
        mapSeqData << QVariant(false);  // A3
        mapSeqData << "";  // A4
        ConnectDataItem* item = new ConnectDataItem( mapSeqData, mItemIDCount++, getItem(index.parent()));
        parents.last()->insertChild(index.row(), item );
        parents << item;
        if (!keys.endsWith("-"))
            keys << "-";

        if (node[i].Type()==YAML::NodeType::Map) {
            qDebug() << " .... 1 " << keys;
           insertSchemaData(schemaname, keys, new ConnectData(node[i]), parents);
        } else if (node[i].Type()==YAML::NodeType::Scalar) {
            qDebug() << " .... 2 " << keys;
            ConnectSchema* schema = mConnect->getSchema(schemaname);
            QString key = QString::fromStdString(node[i].as<std::string>());
            QList<QVariant> itemData;
            itemData << key;
            itemData << "";
            itemData << QVariant((int)DataCheckState::ElementKey);
            itemData << QVariant(schema->getTypeAsStringList(keys.join(":")));
            itemData << QVariant(schema->getAllowedValueAsStringList(keys.join(":")));
            itemData << QVariant(false);  // A0
            itemData << QVariant(false); // A2
            itemData << QVariant(false); // A3
            itemData << ""; // A4
            parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
        }
    }
    endInsertRows();

    informDataChanged(parentIndex);
    qDebug() << "end appendListElement (" << index.row() << "," << index.column() << ") " << rowCount();
}

ConnectData *ConnectDataModel::getConnectData()
{
     YAML::Node root = YAML::Node(YAML::NodeType::Sequence);
     for(int i=0; i<mRootItem->childCount(); ++i) {
         ConnectDataItem* item = mRootItem->child(i);
         YAML::Node node = YAML::Node(YAML::NodeType::Map);
         std::string key = item->data((int)DataItemColumn::Key).toString().toStdString();
         YAML::Node mapnode = YAML::Node(YAML::NodeType::Map);
         getData( item, mapnode );
         node[key] = mapnode;
         root[i] = node;
     }
     return new ConnectData(root);
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
                     index(getItem(parent)->childCount()-1, (int)DataItemColumn::Expand, parent),
                     QVector<int> { Qt::DisplayRole, Qt::ToolTipRole, Qt::DecorationRole} );

}

void ConnectDataModel::setupTreeItemModelData()
{
    QList<QVariant> rootData;
    rootData << "Key" << "Value"
             << "State" << "Type" << "AllowedValue"
             << ""  << "" << "" << "" << "";

    mRootItem = new ConnectDataItem(rootData, mItemIDCount++);

    if (!mConnectData->getRootNode().IsNull()) {
        YAML::Node node = mConnectData->getRootNode();
        Q_ASSERT(node.Type()==YAML::NodeType::Sequence);

        QList<ConnectDataItem*> parents;
        parents << mRootItem;

        int position = 0;
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
                if (position>=parents.last()->childCount()) {
                    parents.last()->appendChild(new ConnectDataItem(listData, mItemIDCount++, parents.last()));
                } else {
                    parents.last()->insertChild(position, new ConnectDataItem(listData, mItemIDCount++, parents.last()));
                }
                parents << parents.last()->child(parents.last()->childCount()-1);

                insertSchemaData(schemaName, dataKeys, new ConnectData(it->second), parents);

                parents.pop_back();
            }
            position++;
        }
        informDataChanged( index(0,0).parent() );
    }
}

void ConnectDataModel::insertSchemaData(const QString& schemaName, const QStringList& keys, ConnectData* data, QList<ConnectDataItem*>& parents)
{
    QStringList dataKeys(keys);
    ConnectSchema* schema = mConnect->getSchema(schemaName);

    for (YAML::const_iterator mit = data->getRootNode().begin(); mit != data->getRootNode().end(); ++mit) {
         QString mapToSequenceKey = "";
         if (mit->second.Type()==YAML::NodeType::Scalar) {
             QString key = QString::fromStdString(mit->first.as<std::string>());
             dataKeys << key;
             QList<QVariant> itemData;
             itemData << key;
             itemData << QVariant(mit->second.as<std::string>().c_str()); // TODO
             itemData << QVariant((int)DataCheckState::ElementValue);
             itemData << QVariant(schema->getTypeAsStringList(dataKeys.join(":"))); //key));
             itemData << QVariant(schema->getAllowedValueAsStringList(dataKeys.join(":"))); //key));
             itemData << QVariant(!schema->isRequired(dataKeys.join(":"))); //key)); // A0
             itemData << QVariant(false); // A2
             itemData << QVariant(false); // A3
             itemData << ""; // A4
             parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));

             dataKeys.removeLast();
         } else if (mit->second.Type()==YAML::NodeType::Map) {
                   QString key = QString::fromStdString(mit->first.as<std::string>());
                   dataKeys << key;
                   qDebug() << " ________map : " << dataKeys.join(":");
                   QList<QVariant> itemData;
                   itemData << key;
                   itemData << ""; // TODO
                   itemData << QVariant((int)DataCheckState::KeyItem);
                   itemData << QVariant(schema->getTypeAsStringList(dataKeys.join(":"))); //key));
                   itemData << QVariant(QStringList());
                   itemData << QVariant(!schema->isRequired(dataKeys.join(":"))); //key)); // A0
                   itemData << QVariant(false); // A2
                   itemData << QVariant(false); // A3
                   itemData << ""; // A4
                   parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
                   parents << parents.last()->child(parents.last()->childCount()-1);
                   int k = 0;
                   for (YAML::const_iterator dmit = mit->second.begin(); dmit != mit->second.end(); ++dmit) {
                       QString mapkey = QString::fromStdString(dmit->first.as<std::string>());
                       dataKeys << mapkey;
                       QList<QVariant> mapitemData;
                       mapitemData << mapkey;
                       Q_ASSERT(dmit->second.Type()==YAML::NodeType::Scalar); // TODO
                       mapitemData << QVariant(dmit->second.as<std::string>().c_str()); // TODO
                       mapitemData << QVariant((int)DataCheckState::ElementMap);
                       mapitemData << QVariant(schema->getTypeAsStringList(dataKeys.join(":"))); //key));
                       mapitemData << QVariant(QStringList());
                       mapitemData << QVariant(!schema->isRequired(dataKeys.join(":"))); //key)); // A0
                       mapitemData << ""; // A2
                       mapitemData << ""; // A3
                       mapitemData << ""; // A4
                       parents.last()->appendChild(new ConnectDataItem(mapitemData, mItemIDCount++, parents.last()));
                       dataKeys.removeLast();
                       k++;
                   }

                   QList<QVariant> sequenceDummyData;
                   sequenceDummyData << key;
                   sequenceDummyData << "";
                   sequenceDummyData << QVariant((int)DataCheckState::MapAppend);
                   sequenceDummyData << QVariant(QStringList());
                   sequenceDummyData << QVariant(QStringList());
                   sequenceDummyData << QVariant(false); // A0
                   sequenceDummyData << QVariant(false); // A2
                   sequenceDummyData << QVariant(false); // A3
                   sequenceDummyData << QVariant(false); // A4
                   parents.last()->appendChild(new ConnectDataItem(sequenceDummyData, mItemIDCount++, parents.last()));

                   parents.pop_back();
                   dataKeys.removeLast();
         } else if (mit->second.Type()==YAML::NodeType::Sequence) {
             QString key = QString::fromStdString(mit->first.as<std::string>());
             mapToSequenceKey = key;
             dataKeys << key;
             qDebug() << key << ">>" << dataKeys;
             QList<QVariant> itemData;
             itemData << key;
             itemData << "";
             itemData << QVariant((int)DataCheckState::KeyItem);
             itemData << QVariant(schema->getTypeAsStringList(dataKeys.join(":"))); //key));
             itemData << QVariant(QStringList());
             itemData << QVariant(!schema->isRequired(dataKeys.join(":"))); // A0
             itemData << ""; // A2
             itemData << ""; // A3
             itemData << ""; // A4
             parents.last()->appendChild(new ConnectDataItem(itemData, mItemIDCount++, parents.last()));
             parents << parents.last()->child(parents.last()->childCount()-1);
             dataKeys << "-";
             for(size_t k = 0; k<mit->second.size(); k++) {
                 QList<QVariant> indexData;
                 indexData << QVariant::fromValue(k);
                 indexData << QVariant(QStringList());
                 indexData << QVariant((int)DataCheckState::ListItem);
                 indexData << QVariant(QStringList());
                 indexData << QVariant(QStringList());
                 indexData << QVariant(!schema->isRequired(dataKeys.join(":"))); // A0
                 indexData << ""; // A2
                 indexData << ""; // A3
                 indexData << ""; // A4
                 parents.last()->appendChild(new ConnectDataItem(indexData, mItemIDCount++, parents.last()));

                        if (mit->second[k].Type()==YAML::NodeType::Map) {
                           parents << parents.last()->child(parents.last()->childCount()-1);
                           const YAML::Node mapnode = mit->second[k];
                           for (YAML::const_iterator mmit = mapnode.begin(); mmit != mapnode.end(); ++mmit) {
                               key =  QString::fromStdString( mmit->first.as<std::string>() );
                               dataKeys << key;
                               if (mmit->second.Type()==YAML::NodeType::Sequence) {
                                   QList<QVariant> seqSeqData;
                                   seqSeqData << key;
                                   seqSeqData << "";
                                   seqSeqData << QVariant((int)DataCheckState::KeyItem);
                                   seqSeqData << QVariant(schema->getTypeAsStringList(key));
                                   seqSeqData << QVariant(QStringList());
                                   seqSeqData << QVariant(!schema->isRequired(dataKeys.join(":"))); //key)); // A0
                                   seqSeqData << ""; // A2
                                   seqSeqData << ""; // A3
                                   seqSeqData << ""; // A4
                                   parents.last()->appendChild(new ConnectDataItem(seqSeqData, mItemIDCount++, parents.last()));

                                   dataKeys << "-";
                                   parents << parents.last()->child(parents.last()->childCount()-1);
                                   for(size_t kk = 0; kk<mmit->second.size(); kk++) {
                                       QList<QVariant> indexSeqData;
                                       indexSeqData << QVariant::fromValue(kk);
                                       indexSeqData << QVariant(QStringList());
                                       indexSeqData << QVariant((int)DataCheckState::ListItem);
                                       indexSeqData << QVariant(QStringList());
                                       indexSeqData << QVariant(QStringList());
                                       indexSeqData << QVariant(!schema->isRequired(dataKeys.join(":"))); // A0
                                       indexSeqData << ""; // A2
                                       indexSeqData << ""; // A3
                                       indexSeqData << ""; // A4
                                       parents.last()->appendChild(new ConnectDataItem(indexSeqData, mItemIDCount++, parents.last()));

                                       if (mmit->second[kk].Type()==YAML::NodeType::Scalar) {
                                           parents << parents.last()->child(parents.last()->childCount()-1);
                                            QList<QVariant> indexScalarData;
                                           indexScalarData << mmit->second[kk].as<std::string>().c_str();
                                           indexScalarData << ""; // TODO
                                           indexScalarData << QVariant((int)DataCheckState::ElementKey);
                                           indexScalarData << QVariant(QStringList());
                                           indexScalarData << QVariant(QStringList());
                                           indexScalarData << "";  // A0
                                           indexScalarData << "";  // A2
                                           indexScalarData << "";  // A3
                                           indexScalarData << "";  // A4
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
                                        indexSeqDummyData << keys; //QVariant(QStringList());
                                        indexSeqDummyData << QVariant(false);  // A0
                                        indexSeqDummyData << QVariant(false);  // A2
                                        indexSeqDummyData << QVariant(false);  // A3
                                        indexSeqDummyData << QVariant(false);  // A4
                                        parents.last()->appendChild(new ConnectDataItem(indexSeqDummyData, mItemIDCount++, parents.last()));

                                   }
                                   parents.pop_back();
                                   dataKeys.removeLast();

                               } else if (mmit->second.Type()==YAML::NodeType::Map) {
                                          QList<QVariant> mapData;
                                          mapData << key;
                                          mapData << "";
                                          mapData << QVariant((int)DataCheckState::KeyItem);
                                          mapData << QVariant(schema->getTypeAsStringList(dataKeys.join(":")));
                                          mapData << QVariant(QStringList());
                                          mapData << QVariant(!schema->isRequired(dataKeys.join(":"))); // A0
                                          mapData << QVariant(false); // A2
                                          mapData << ""; // A3
                                          mapData << ""; // A4
                                          parents.last()->appendChild(new ConnectDataItem(mapData, mItemIDCount++, parents.last()));

                                          parents << parents.last()->child(parents.last()->childCount()-1);
                                          const YAML::Node mapmapnode = mmit->second;
                                          for (YAML::const_iterator mmmit = mapmapnode.begin(); mmmit != mapmapnode.end(); ++mmmit) {
                                               QList<QVariant> mapSeqData;
                                               mapSeqData << mmmit->first.as<std::string>().c_str();
                                               mapSeqData << mmmit->second.as<std::string>().c_str();  // can be int/bool/double
                                               mapSeqData << QVariant((int)DataCheckState::ElementMap);
                                               mapSeqData << QVariant(schema->getTypeAsStringList(dataKeys.join(":")));
                                               mapSeqData << QVariant(QVariant(schema->getAllowedValueAsStringList(dataKeys.join(":"))));
                                               mapSeqData << QVariant(!schema->isRequired(dataKeys.join(":")));  // A0
                                               mapSeqData << "";  // A2
                                               mapSeqData << "";  // A3
                                               mapSeqData << "";  // A4
                                               parents.last()->appendChild(new ConnectDataItem(mapSeqData, mItemIDCount++, parents.last()));
//                                                       dataKeys.removeLast();
                                          }
                                          QList<QVariant> indexSeqDummyData;
                                          indexSeqDummyData << key;
                                          indexSeqDummyData << "";
                                          indexSeqDummyData << QVariant((int)DataCheckState::MapAppend);
                                          indexSeqDummyData << QVariant(QStringList());
                                          indexSeqDummyData << QVariant(QStringList());
                                          indexSeqDummyData << QVariant(false);  // A0
                                          indexSeqDummyData << QVariant(false);  // A2
                                          indexSeqDummyData << QVariant(false);  // A3
                                          indexSeqDummyData << QVariant(false);  // A4
                                          parents.last()->appendChild(new ConnectDataItem(indexSeqDummyData, mItemIDCount++, parents.last()));

                                          parents.pop_back();

                               } else if (mmit->second.Type()==YAML::NodeType::Scalar) {
                                     QList<QVariant> mapSeqData;
                                     mapSeqData << key;
                                     mapSeqData << mmit->second.as<std::string>().c_str(); // TODO
                                     mapSeqData << QVariant((int)DataCheckState::ElementValue);
                                     mapSeqData << QVariant(schema->getTypeAsStringList(dataKeys.join(":")));
                                     mapSeqData << QVariant(QVariant(schema->getAllowedValueAsStringList(dataKeys.join(":"))));
                                     mapSeqData << QVariant(!schema->isRequired(dataKeys.join(":")));  // A0
                                     mapSeqData << "";  // A2
                                     mapSeqData << "";  // A3
                                     mapSeqData << "";  // A4
                                     parents.last()->appendChild(new ConnectDataItem(mapSeqData, mItemIDCount++, parents.last()));
                               }
                               dataKeys.removeLast();
                           }
                           parents.pop_back();
                        } else if (mit->second[k].Type()==YAML::NodeType::Scalar) {
                                   parents << parents.last()->child(parents.last()->childCount()-1);
                                   QList<QVariant> mapSeqData;
                                   mapSeqData << QVariant( QString::fromStdString(mit->second[k].as<std::string>()) );
                                   mapSeqData << ""; // TODO
                                   mapSeqData << QVariant((int)DataCheckState::ElementKey);
                                   mapSeqData << QVariant(QStringList());
                                   mapSeqData << QVariant(QStringList());
                                   mapSeqData << "";  // A0
                                   mapSeqData << "";  // A2
                                   mapSeqData << "";  // A3
                                   mapSeqData << "";  // A4
                                   parents.last()->appendChild(new ConnectDataItem(mapSeqData, mItemIDCount++, parents.last()));
                                   parents.pop_back();
                        }
                    }
                    dataKeys.removeLast();

                    QList<QVariant> sequenceDummyData;
                    sequenceDummyData << mapToSequenceKey;
                    sequenceDummyData << "";
                    sequenceDummyData << QVariant((int)DataCheckState::ListAppend);
                    QStringList keys(dataKeys);
                    keys.insert(0,schemaName);
                    sequenceDummyData << QVariant(QStringList());
                    sequenceDummyData << keys; //QVariant(QStringList());
                    sequenceDummyData << QVariant(false); // A0
                    sequenceDummyData << QVariant(false); // A2
                    sequenceDummyData << QVariant(false); // A3
                    sequenceDummyData << QVariant(false); // A4
                    parents.last()->appendChild(new ConnectDataItem(sequenceDummyData, mItemIDCount++, parents.last()));

                    parents.pop_back();
                    dataKeys.removeLast();
             }
    }
}

} // namespace connect
} // namespace studio
} // namespace gams