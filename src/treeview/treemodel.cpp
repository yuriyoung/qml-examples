/**
 * QML examples - Qt5 and QML examples
 * Copyright (c) 2019 Yuri Young<yuri.young@qq.com>
 *
 * This examples is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This examples is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "treemodel.h"
#include "treemodel_p.h"

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *child)
{
    m_childItems.append(child);
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;
    for (int row = 0; row < count; ++row)
    {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        m_childItems.insert(position, item);
    }

    return true;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.insert(position, QVariant());

    foreach (TreeItem *child, m_childItems)
        child->insertColumns(position, columns);

    return true;
}

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.remove(position);

    foreach (TreeItem *child, m_childItems)
        child->removeColumns(position, columns);

    return true;
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

TreeItem *TreeItem::parent()
{
    return m_parentItem;
}

bool TreeItem::hasChildren() const
{
    return !m_childItems.isEmpty();
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= m_itemData.size())
        return false;

    m_itemData[column] = value;
    return true;
}

/**
 * @brief TreeModelPrivate::TreeModelPrivate
 */
TreeModelPrivate::TreeModelPrivate()
    : rootItem(new TreeItem(QVector<QVariant>(1, "root")))
{

}

TreeModelPrivate::~TreeModelPrivate()
{
    delete rootItem;
}

TreeItem *TreeModelPrivate::treeItemOf(const QModelIndex &index) const
{
    if (index.isValid())
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

/**
 * @brief TreeModel::TreeModel
 * @param parent
 */
TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , d_ptr(new TreeModelPrivate())
{

}

TreeModel::TreeModel(TreeModelPrivate &dd, QObject *parent)
    : QAbstractItemModel(parent)
    , d_ptr(&dd)
{
    Q_D(TreeModel);
    d->q_ptr = this;
}

TreeModel::~TreeModel()
{

}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const TreeModel);
    TreeItem *parentItem = d->treeItemOf(parent);
    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_D(const TreeModel);
    if (parent.isValid())
        return d->treeItemOf(parent)->columnCount();
    else
        return d->rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    Q_D(const TreeModel);

    if (!index.isValid())
        return QVariant();

    if (role & ~(Qt::DisplayRole | Qt::EditRole))
        return QVariant();

    TreeItem *item = d->treeItemOf(index);
    return item->data(index.column());
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(TreeModel);
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    TreeItem *item = d->treeItemOf(index);
    bool success =  item->setData(index.column(), value);
    if(success)
        emit dataChanged(index, index);

    return success;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const TreeModel);
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem = d->treeItemOf(parent);
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    Q_D(const TreeModel);
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = d->treeItemOf(index);
    Q_ASSERT(childItem);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == d->rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_D(TreeModel);
    if(row < 0 || row > rowCount(parent))
        row = rowCount(parent);

    TreeItem *parentItem = d->treeItemOf(parent);

    bool success = false;
    beginInsertRows(parent, row, row + count - 1);
    success = parentItem->insertChildren(row, count, d->rootItem->columnCount());
    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_D(TreeModel);

    if(row < 0 || count <= 0)
        return false;
    else if (row + count > rowCount(parent))
        count = rowCount(parent) - row;

    if(!count)
        return false;

    TreeItem *parentItem = d->treeItemOf(parent);

    bool success = false;
    beginRemoveRows(parent, row, row + count - 1);
    success = parentItem->removeChildren(row, count);
    endRemoveRows();

    return true;
}

bool TreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    Q_D(TreeModel);
    if(!count)
        return false;

    if(column < 0 || column > columnCount(parent))
        column = columnCount(parent);

    bool success = false;
    beginInsertColumns(parent, column, column + count - 1);
    success = d->rootItem->insertColumns(column, count);
    endInsertColumns();

    return success;
}

bool TreeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    Q_D(TreeModel);
    if(column < 0 || count <= 0)
        return false;
    else if (column + count > columnCount(parent))
        count = columnCount(parent) - column;

    if(!count)
        return false;

    bool success = false;
    beginRemoveColumns(parent, column, column + count - 1);
    success = d->rootItem->removeColumns(column, count);
    endRemoveColumns();

    if (d->rootItem->columnCount() == 0)
        removeRows(0, rowCount(parent));

    return success;
}

void TreeModel::clear()
{
    Q_D(TreeModel);
    if(d->rootItem)
    {
        beginResetModel();
        delete d->rootItem;
        d->rootItem = nullptr;
        endResetModel();
        d->rootItem = new TreeItem(QVector<QVariant>(1, "root"));
    }
}
