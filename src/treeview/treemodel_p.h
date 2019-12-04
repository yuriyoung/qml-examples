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
#ifndef TREEMODEL_P_H
#define TREEMODEL_P_H

#include "treemodel.h"

class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    bool insertChildren(int position, int count, int columns);
    bool removeChildren(int position, int count);

    bool insertColumns(int position, int columns);
    bool removeColumns(int position, int columns);

    TreeItem *child(int row);
    TreeItem *parent();
    bool hasChildren() const;

    int childCount() const;
    int columnCount() const;

    int row() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

public:
    QPersistentModelIndex index;
    int depth = 0;
    bool expanded = false;

private:
    QList<TreeItem*> m_childItems;
    QVector<QVariant> m_itemData;
    TreeItem *m_parentItem;
};

class TreeModelPrivate
{
    Q_DECLARE_PUBLIC(TreeModel)
public:
    TreeModelPrivate();
    virtual ~TreeModelPrivate();
    TreeItem *treeItemOf(const QModelIndex &index) const;

    TreeItem *rootItem;

protected:
    TreeModel *q_ptr;
};
#endif // TREEMODEL_P_H
