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
#include "treemodelproxy.h"

#include <QPointer>
#include <QSet>

class TreeModelProxyPrivate
{
    Q_DISABLE_COPY(TreeModelProxyPrivate)
    Q_DECLARE_PUBLIC(TreeModelProxy)
public:
    enum
    {
        DepthRole = Qt::UserRole - 5,
        ExpandedRole,
        HasChildrenRole,
        HasSiblingRole,
        ModelIndexRole
    };

    QPointer<QAbstractItemModel> model;
    QPersistentModelIndex rootIndex;
    QSet<QPersistentModelIndex> expandedItems;

protected:
    TreeModelProxy *q_ptr;
};

TreeModelProxy::TreeModelProxy(QObject *parent)
    : QAbstractListModel(parent)
{

}

TreeModelProxy::~TreeModelProxy()
{

}

QHash<int, QByteArray> TreeModelProxy::roleNames() const
{
    Q_D(const TreeModelProxy);
    if(!d->model)
        return QHash<int, QByteArray>();

    QHash<int, QByteArray> modelRoleNames = d->model->roleNames();
    modelRoleNames.insert(TreeModelProxyPrivate::DepthRole,         "_TreeView_ItemDepth");
    modelRoleNames.insert(TreeModelProxyPrivate::ExpandedRole,      "_TreeView_ItemExpanded");
    modelRoleNames.insert(TreeModelProxyPrivate::HasChildrenRole,   "_TreeView_HasChildren");
    modelRoleNames.insert(TreeModelProxyPrivate::HasSiblingRole,    "_TreeView_HasSibling");
    modelRoleNames.insert(TreeModelProxyPrivate::ModelIndexRole,    "_TreeView_ModelIndex");
    return modelRoleNames;
}

int TreeModelProxy::rowCount(const QModelIndex &parent) const
{
    Q_D(const TreeModelProxy);
    Q_UNUSED(parent)

    // TODO: children count

    return 0;
}

int TreeModelProxy::columnCount(const QModelIndex &parent) const
{
    Q_D(const TreeModelProxy);

    return d->model->columnCount(parent);
}

QVariant TreeModelProxy::data(const QModelIndex &index, int role) const
{
    Q_D(const TreeModelProxy);

    if (!d->model || !index.isValid())
        return QVariant();

    // TODO: tree props value

    return d->model->data(index, role);
}

bool TreeModelProxy::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(TreeModelProxy);
    if (!d->model || !index.isValid())
        return false;

    // TODO: set tree props value

    return d->model->setData(index, value, role);
}

QAbstractItemModel *TreeModelProxy::model() const
{
    Q_D(const TreeModelProxy);

    return d->model;// ? QVariant::fromValue(d->model) : d->modelVariant;
}

void TreeModelProxy::setModel(QAbstractItemModel *model)
{
    Q_D(TreeModelProxy);
//    QVariant model = m;
//    if (model.userType() == qMetaTypeId<QJSValue>())
//        model = model.value<QJSValue>().toVariant();

    if(!model)
        return;

    if (d->model == model)
        return;

    // TODO:
    // binding a new model
    // connects signals & slots for new model
    // refresh new model data

    emit modelChanged();
}

void TreeModelProxy::expand(const QModelIndex &index)
{
    Q_D(TreeModelProxy);
    if (!d->model)
        return;

    Q_ASSERT(!index.isValid() || index.model() == d->model);
    if (!index.isValid() || !d->model->hasChildren(index))
        return;
    if (d->expandedItems.contains(index))
        return;

    // TODO: load hidden items and show them

    emit expanded(index);
}

void TreeModelProxy::collapse(const QModelIndex &index)
{
    Q_D(TreeModelProxy);
    if (!d->model)
        return;

    Q_ASSERT(!index.isValid() || index.model() == d->model);
    if (!index.isValid() || !d->model->hasChildren(index))
        return;
    if (!d->expandedItems.contains(index))
        return;

    // TODO: remove/hide items

    emit collapsed(index);
}
