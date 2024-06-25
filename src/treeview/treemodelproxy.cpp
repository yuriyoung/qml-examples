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

    class TreeItem
    {
    public:
        explicit TreeItem(const QModelIndex &idx = QModelIndex(), int d = 0, int e = false)
            : index(idx), depth(d), expanded(e)
        { }

        inline bool operator== (const TreeItem &other) const
        {
            return this->index == other.index;
        }

        QPersistentModelIndex index;
        int depth;
        bool expanded;
    };

    TreeModelProxyPrivate();

    void clearModelData();
    const QModelIndex &mapToModel(const QModelIndex &index) const;
    void showTopLevels(bool doInsertRows = true);
    void showChildren(const TreeItem &parentItem, int start, int end, bool doInsert = true, bool doExpandPending = true);
    void expandPendingRows(bool doInsertRows = true);

    QPointer<QAbstractItemModel> model;
    QPersistentModelIndex rootIndex;
    QSet<QPersistentModelIndex> expandedItems;
    QList<TreeItem> items;
    QList<TreeItem *> itemsToExpand;
    mutable int lastItemIndex;

protected:
    TreeModelProxy *q_ptr;
};

TreeModelProxyPrivate::TreeModelProxyPrivate()
    : model(nullptr), rootIndex(QModelIndex()), lastItemIndex(0)
{

}

void TreeModelProxyPrivate::clearModelData()
{
    items.clear();
    expandedItems.clear();
    rootIndex = QModelIndex();
    lastItemIndex = 0;
}

const QModelIndex &TreeModelProxyPrivate::mapToModel(const QModelIndex &index) const
{
    return items.at(index.row()).index;
}

void TreeModelProxyPrivate::showTopLevels(bool doInsertRows)
{
    if (!model)
        return;

    if (model->hasChildren(rootIndex) && model->canFetchMore(rootIndex))
        model->fetchMore(rootIndex);
    const int topLevelRowCount = model->rowCount(rootIndex);
    if (topLevelRowCount == 0)
        return;

    showChildren(TreeItem(rootIndex), 0, topLevelRowCount - 1, doInsertRows);
}

void TreeModelProxyPrivate::showChildren(const TreeModelProxyPrivate::TreeItem &parentItem, int start, int end, bool doInsert, bool doExpandPending)
{
    Q_Q(TreeModelProxy);
    const QModelIndex &parentIndex = parentItem.index;
    int rowIdx = parentIndex.isValid() && parentIndex != rootIndex ? q->itemIndex(parentIndex) + 1 : 0;
    Q_ASSERT(rowIdx == 0 || parentItem.expanded);
    if (parentIndex.isValid() && parentIndex != rootIndex && (rowIdx == 0 || !parentItem.expanded))
        return;

    if (model->rowCount(parentIndex) == 0) {
        if (model->hasChildren(parentIndex) && model->canFetchMore(parentIndex))
            model->fetchMore(parentIndex);
        return;
    }

    int insertCount = end - start + 1;
    int startIdx;
    if (start == 0) {
        startIdx = rowIdx;
    } else {
        const QModelIndex &prevSiblingIdx = model->index(start - 1, 0, parentIndex);
        startIdx = q->lastChildIndex(prevSiblingIdx) + 1;
    }

    int rowDepth = rowIdx == 0 ? 0 : parentItem.depth + 1;
    if (doInsert)
        q->beginInsertRows(QModelIndex(), startIdx, startIdx + insertCount - 1);
    items.reserve(items.count() + insertCount);
    for (int i = 0; i < insertCount; i++) {
        const QModelIndex &cmi = model->index(start + i, 0, parentIndex);
        bool expanded = expandedItems.contains(cmi);
        items.insert(startIdx + i, TreeItem(cmi, rowDepth, expanded));
        if (expanded)
            itemsToExpand.append(&items[startIdx + i]);
    }
    if (doInsert)
        q->endInsertRows();

    if (doExpandPending)
        expandPendingRows(doInsert);
}

void TreeModelProxyPrivate::expandPendingRows(bool doInsertRows)
{
    while (!itemsToExpand.isEmpty())
    {
        TreeItem *item = itemsToExpand.takeFirst();
        Q_ASSERT(item->expanded);
        const QModelIndex &index = item->index;
        int childrenCount = model->rowCount(index);
        if (childrenCount == 0)
        {
            if (model->hasChildren(index) && model->canFetchMore(index))
                model->fetchMore(index);
            continue;
        }
        showChildren(*item, 0, childrenCount - 1, doInsertRows, false);
    }
}

/**
 * @brief TreeModelProxy::TreeModelProxy
 * @param parent
 */
TreeModelProxy::TreeModelProxy(QObject *parent)
    : TreeModelProxy(*new TreeModelProxyPrivate(), parent)
{

}

TreeModelProxy::TreeModelProxy(TreeModelProxyPrivate &dd, QObject *parent)
    : QAbstractItemModel(parent)
    , d_ptr(&dd)
{
    Q_D(TreeModelProxy);
    d->q_ptr = this;
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

    return d->items.count();
}

int TreeModelProxy::columnCount(const QModelIndex &parent) const
{
    Q_D(const TreeModelProxy);
    if(!d->model)
        return 0;

    return d->model->columnCount(parent);
}

QVariant TreeModelProxy::data(const QModelIndex &index, int role) const
{
    Q_D(const TreeModelProxy);

    if (!d->model || !index.isValid())
        return QVariant();

    const QModelIndex &modelIndex = d->mapToModel(index);
    if(!modelIndex.isValid())
        return QVariant();

    switch (role)
    {
    case TreeModelProxyPrivate::DepthRole:
        return d->items.at(index.row()).depth;
    case TreeModelProxyPrivate::ExpandedRole:
        return isExpanded(index.row());
    case TreeModelProxyPrivate::HasChildrenRole:
        return d->model->hasChildren(modelIndex);
        // return (modelIndex.flags() & Qt::ItemNeverHasChildren) && d->model->hasChildren(modelIndex);
    case TreeModelProxyPrivate::HasSiblingRole:
        return modelIndex.row() != d->model->rowCount(modelIndex.parent()) - 1;
    case TreeModelProxyPrivate::ModelIndexRole:
        return modelIndex;
    default:
        return d->model->data(modelIndex, role);
    }
}

bool TreeModelProxy::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(TreeModelProxy);
    if (!d->model || !index.isValid())
        return false;

    switch (role)
    {
    case TreeModelProxyPrivate::DepthRole:
    case TreeModelProxyPrivate::ExpandedRole:
    case TreeModelProxyPrivate::HasChildrenRole:
    case TreeModelProxyPrivate::HasSiblingRole:
    case TreeModelProxyPrivate::ModelIndexRole:
        return false;
    default:
    {
        const QModelIndex &pmi = d->mapToModel(index);
        return d->model->setData(pmi, value, role);
    }
    }
}

QModelIndex TreeModelProxy::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex TreeModelProxy::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant TreeModelProxy::model() const
{
    Q_D(const TreeModelProxy);

    return QVariant::fromValue(d->model); //d->model ? QVariant::fromValue(d->model) : d->modelVariant;
}

void TreeModelProxy::setModel(const QVariant &model)
{
    Q_D(TreeModelProxy);

    QObject *object = qvariant_cast<QObject *>(QVariant::fromValue(model));
    if(!object)
        return;

    if(!qobject_cast<QAbstractItemModel *>(object))
        return;

    if (d->model == object)
        return;

    struct SignalSlot {
        const char *signal;
        const char *slot;
    };
    static const SignalSlot connections[] = {
        { SIGNAL(destroyed(QObject*)),
          SLOT(modelHasBeenDestroyed()) },
        { SIGNAL(modelReset()),
          SLOT(modelHasBeenReset()) },
        { SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)),
          SLOT(modelDataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)) },

        { SIGNAL(layoutAboutToBeChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)),
          SLOT(modelLayoutAboutToBeChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)) },
        { SIGNAL(layoutChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)),
          SLOT(modelLayoutChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)) },

        { SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)),
          SLOT(modelRowsAboutToBeInserted(const QModelIndex &, int, int)) },
        { SIGNAL(rowsInserted(const QModelIndex&, int, int)),
          SLOT(modelRowsInserted(const QModelIndex&, int, int)) },
        { SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)),
          SLOT(modelRowsAboutToBeRemoved(const QModelIndex&, int, int)) },
        { SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
          SLOT(modelRowsRemoved(const QModelIndex&, int, int)) },
        { SIGNAL(rowsAboutToBeMoved(const QModelIndex&, int, int, const QModelIndex&, int)),
          SLOT(modelRowsAboutToBeMoved(const QModelIndex&, int, int, const QModelIndex&, int)) },
        { SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)),
          SLOT(modelRowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)) },
        { nullptr, nullptr }
    };

    if (d->model)
    {
        for (const SignalSlot *ss = &connections[0]; ss->signal; ss++)
            disconnect(d->model, ss->signal, this, ss->slot);
    }

    QAbstractItemModel *oldModel = d->model;

    clear();
    d->model = nullptr;
    delete oldModel;
    d->model = qobject_cast<QAbstractItemModel *>(object);

    if (d->model)
    {
        for (const SignalSlot *ss = &connections[0]; ss->signal; ss++)
            connect(d->model, ss->signal, this, ss->slot);

        d->showTopLevels();
    }

    emit modelChanged();
}

const QModelIndex &TreeModelProxy::rootIndex() const
{
    Q_D(const TreeModelProxy);
    return d->rootIndex;
}

void TreeModelProxy::setRootIndex(const QModelIndex &index)
{
    Q_D(TreeModelProxy);
    if (d->rootIndex == index)
        return;

    if (d->model)
        clear();

    d->rootIndex = index;

    if (d->model)
        d->showTopLevels(true);

    emit rootIndexChanged();
}

void TreeModelProxy::resetRootIndex()
{
    setRootIndex(QModelIndex());
}

int TreeModelProxy::itemIndex(const QModelIndex &index) const
{
    Q_D(const TreeModelProxy);

    // This is basically a plagiarism of QTreeViewPrivate::viewIndex()
    if (!index.isValid() || index == d->rootIndex || d->items.isEmpty())
        return -1;

    const int totalCount = d->items.count();

    // We start nearest to the lastViewedItem
    int localCount = qMin(d->lastItemIndex - 1, totalCount - d->lastItemIndex);
    for (int i = 0; i < localCount; ++i)
    {
        const TreeModelProxyPrivate::TreeItem &item1 = d->items.at(d->lastItemIndex + i);
        if (item1.index == index)
        {
            d->lastItemIndex = d->lastItemIndex + i;
            return d->lastItemIndex;
        }
        const TreeModelProxyPrivate::TreeItem &item2 = d->items.at(d->lastItemIndex - i - 1);
        if (item2.index == index)
        {
            d->lastItemIndex = d->lastItemIndex - i - 1;
            return d->lastItemIndex;
        }
    }

    for (int j = qMax(0, d->lastItemIndex + localCount); j < totalCount; ++j)
    {
        const TreeModelProxyPrivate::TreeItem &item = d->items.at(j);
        if (item.index == index)
        {
            d->lastItemIndex = j;
            return j;
        }
    }
    for (int j = qMin(totalCount, d->lastItemIndex - localCount) - 1; j >= 0; --j) {
        const TreeModelProxyPrivate::TreeItem &item = d->items.at(j);
        if (item.index == index)
        {
            d->lastItemIndex = j;
            return j;
        }
    }

    // nothing found
    return -1;
}

int TreeModelProxy::lastChildIndex(const QModelIndex &index)
{
    Q_D(TreeModelProxy);
    if (!d->expandedItems.contains(index))
        return itemIndex(index);

    QModelIndex parent = index.parent();
    QModelIndex nextSiblingIndex;
    while (parent.isValid())
    {
        nextSiblingIndex = parent.sibling(parent.row() + 1, 0);
        if (nextSiblingIndex.isValid())
            break;
        parent = parent.parent();
    }

    int firstIndex = nextSiblingIndex.isValid() ? itemIndex(nextSiblingIndex) : d->items.count();
    return firstIndex - 1;
}

void TreeModelProxy::expandRow(int row)
{
    Q_D(TreeModelProxy);
    if (!d->model || isExpanded(row))
        return;

    TreeModelProxyPrivate::TreeItem &item = d->items[row];
    if ((item.index.flags() & Qt::ItemNeverHasChildren) || !d->model->hasChildren(item.index))
        return;
    item.expanded = true;
    d->expandedItems.insert(item.index);
    QVector<int> changedRole(1, TreeModelProxyPrivate::ExpandedRole);
    emit dataChanged(index(row, 0), index(row, 0), changedRole);

    d->itemsToExpand.append(&item);
    d->expandPendingRows();
}

void TreeModelProxy::collapseRow(int row)
{
    Q_D(TreeModelProxy);
    if (!d->model || !isExpanded(row))
        return;

    TreeModelProxyPrivate::TreeItem &item = d->items[row];
    item.expanded = false;
    d->expandedItems.remove(item.index);
    QVector<int> changedRole(1, TreeModelProxyPrivate::ExpandedRole);
    emit dataChanged(index(row, 0), index(row, 0), changedRole);
    int childrenCount = d->model->rowCount(item.index);
    if ((item.index.flags() & Qt::ItemNeverHasChildren) || !d->model->hasChildren(item.index) || childrenCount == 0)
        return;

    const QModelIndex &emi = d->model->index(childrenCount - 1, 0, item.index);
    int lastIndex = lastChildIndex(emi);
    int startIndex = row + 1;
    removeVisible(startIndex, lastIndex);
}

bool TreeModelProxy::isExpanded(int row) const
{
    Q_D(const TreeModelProxy);
    return d->items.at(row).expanded;
}

bool TreeModelProxy::isVisible(const QModelIndex &index)
{
    return itemIndex(index) != -1;
}

bool TreeModelProxy::childrenVisible(const QModelIndex &index)
{
    Q_D(TreeModelProxy);

    return (index == d->rootIndex && !d->items.isEmpty())
               || (d->expandedItems.contains(index) && isVisible(index));
}

bool TreeModelProxy::removeVisible(int startIndex, int endIndex, bool destroyed)
{
    Q_D(TreeModelProxy);

    if (startIndex < 0 || endIndex < 0 || startIndex > endIndex)
            return false;

    if (destroyed)
        beginRemoveRows(QModelIndex(), startIndex, endIndex);

    d->items.erase(d->items.begin() + startIndex, d->items.begin() + endIndex + 1);

    if (destroyed)
        endRemoveRows();

    return true;
}

QItemSelection TreeModelProxy::selectionRange(const QModelIndex &fromIndex, const QModelIndex &toIndex) const
{
    Q_D(const TreeModelProxy);
    int from = itemIndex(fromIndex);
    int to = itemIndex(toIndex);
    if (from == -1)
    {
        if (to == -1)
            return QItemSelection();
        return QItemSelection(toIndex, toIndex);
    }
    if (from > to)
        qSwap(from, to);

    typedef QPair<QModelIndex, QModelIndex> MIPair;
    typedef QHash<QModelIndex, MIPair> MI2MIPairHash;
    MI2MIPairHash ranges;
    QModelIndex firstIndex = d->items.at(from).index;
    QModelIndex lastIndex = firstIndex;
    QModelIndex previousParent = firstIndex.parent();
    bool selectLastRow = false;
    for (int i = from + 1; i <= to || (selectLastRow = true); i++)
    {
        // We run an extra iteration to make sure the last row is
        // added to the selection. (And also to avoid duplicating
        // the insertion code.)
        QModelIndex index;
        QModelIndex parent;
        if (!selectLastRow)
        {
            index = d->items.at(i).index;
            parent = index.parent();
        }
        if (selectLastRow || previousParent != parent)
        {
            const MI2MIPairHash::iterator &it = ranges.find(previousParent);
            if (it == ranges.end())
                ranges.insert(previousParent, MIPair(firstIndex, lastIndex));
            else
                it->second = lastIndex;

            if (selectLastRow)
                break;

            firstIndex = index;
            previousParent = parent;
        }
        lastIndex = index;
    }

    QItemSelection sel;
    sel.reserve(ranges.count());
    for (const MIPair &pair : std::as_const(ranges))
       sel.append(QItemSelectionRange(pair.first, pair.second));

    return sel;
}

QModelIndex TreeModelProxy::indexOf(int row) const
{
    Q_D(const TreeModelProxy);
    if (!d->model)
        return QModelIndex();

    if (row < 0 || row >= d->items.count())
        return QModelIndex();

    return d->items.at(row).index;
}

bool TreeModelProxy::isExpanded(const QModelIndex &index) const
{
    Q_D(const TreeModelProxy);

    if (!d->model)
        return false;
    Q_ASSERT(!index.isValid() || index.model() == d->model);
    return !index.isValid() || d->expandedItems.contains(index);
}

void TreeModelProxy::clear()
{
    Q_D(TreeModelProxy);

    beginResetModel();
    d->clearModelData();
    endResetModel();
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

    int row = itemIndex(index);
    if (row != -1)
        expandRow(row);
    else
        d->expandedItems.insert(index);

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

    int row = itemIndex(index);
    if (row != -1)
        collapseRow(row);
    else
        d->expandedItems.remove(index);

    emit collapsed(index);
}

QModelIndex TreeModelProxy::add(const QModelIndex &parent)
{
    Q_D(TreeModelProxy);
    if (!d->model)
        return QModelIndex();

    if(parent.model() != d->model)
        return QModelIndex();

    if(d->model->columnCount(parent) == 0)
    {
        if(!d->model->insertColumn(0, parent))
            return QModelIndex();
    }

    if(!d->model->insertRow(0, parent))
        return QModelIndex();

    for (int column = 0; column < d->model->columnCount(parent); ++column)
    {
        QModelIndex child = d->model->index(0, column, parent);
        d->model->setData(child, QVariant("[New Title]"), Qt::EditRole);
    }

    return d->model->index(0, 0, parent);
}

bool TreeModelProxy::remove(int row, int count)
{
    Q_D(TreeModelProxy);
    if (!d->model)
        return false;

    const TreeModelProxyPrivate::TreeItem &item = d->items.at(row);
    QModelIndex index = item.index;
    if (!index.isValid())
        return false;

    return d->model->removeRows(index.row(), count, index.parent());
}

/////////////////////////////////////////////////////////////////////
/// private slots handling
/////////////////////////////////////////////////////////////////////

void TreeModelProxy::modelHasBeenDestroyed()
{
    clear();
    emit modelChanged();
}

void TreeModelProxy::modelHasBeenReset()
{
    Q_D(TreeModelProxy);
    clear();
    d->showTopLevels();
}

void TreeModelProxy::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRigth, const QVector<int> &roles)
{
    Q_D(const TreeModelProxy);

    Q_ASSERT(topLeft.parent() == bottomRigth.parent());
    const QModelIndex &parent = topLeft.parent();
    if (parent.isValid() && !childrenVisible(parent))
        return;

    int topIndex = itemIndex(topLeft);
    if (topIndex == -1) // 'parent' is not visible anymore, though it's been expanded previously
        return;
    for (int i = topLeft.row(); i <= bottomRigth.row(); i++)
    {
        // Group items with same parent to minize the number of 'dataChanged()' emits
        int bottomIndex = topIndex;
        while (bottomIndex < d->items.count())
        {
            const QModelIndex &idx = d->items.at(bottomIndex).index;
            if (idx.parent() != parent) {
                --bottomIndex;
                break;
            }
            if (idx.row() == bottomRigth.row())
                break;
            ++bottomIndex;
        }
        emit dataChanged(index(topIndex, topLeft.column()), index(bottomIndex, topLeft.column()), roles);

        i += bottomIndex - topIndex;
        if (i == bottomRigth.row())
            break;
        topIndex = bottomIndex + 1;
        while (topIndex < d->items.count() && d->items.at(topIndex).index.parent() != parent)
            topIndex++;
    }
}

void TreeModelProxy::modelLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint)
{
    Q_UNUSED(parents)
    Q_UNUSED(hint)
}

void TreeModelProxy::modelLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint)
{
    Q_D(TreeModelProxy);
    Q_UNUSED(hint)

    if (parents.isEmpty()) {
        d->items.clear();
        d->showTopLevels(false /*doInsertRows*/);
        emit dataChanged(index(0, 0), index(d->items.count() - 1, 0));
    }

    for (const QPersistentModelIndex &pmi : parents) {
        if (d->expandedItems.contains(pmi)) {
            int row = itemIndex(pmi);
            if (row != -1) {
                int rowCount = d->model->rowCount(pmi);
                if (rowCount > 0) {
                    const QModelIndex &lmi = d->model->index(rowCount - 1, 0, pmi);
                    int lastRow = lastChildIndex(lmi);
                    removeVisible(row + 1, lastRow, false /*doRemoveRows*/);
                    d->showChildren(d->items.at(row), 0, rowCount - 1, false /*doInsertRows*/);
                    emit dataChanged(index(row + 1, 0), index(lastRow, 0));
                }
            }
        }
    }
}

void TreeModelProxy::modelRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
}

void TreeModelProxy::modelRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
    Q_D(TreeModelProxy);

    if (!childrenVisible(sourceParent))
        return; // Do nothing now. See modelRowsMoved() below.

    if (!childrenVisible(destinationParent))
    {
        modelRowsAboutToBeRemoved(sourceParent, sourceStart, sourceEnd);
    }
    else
    {
        int depthDifference = -1;
        if (destinationParent.isValid()) {
            int destParentIndex = itemIndex(destinationParent);
            depthDifference = d->items.at(destParentIndex).depth;
        }
        if (sourceParent.isValid()) {
            int sourceParentIndex = itemIndex(sourceParent);
            depthDifference -= d->items.at(sourceParentIndex).depth;
        }
        else
        {
            depthDifference++;
        }

        int startIndex = itemIndex(d->model->index(sourceStart, 0, sourceParent));
        const QModelIndex &emi = d->model->index(sourceEnd, 0, sourceParent);
        int endIndex = -1;
        if (isExpanded(emi))
        {
            int rowCount = d->model->rowCount(emi);
            if (rowCount > 0)
                endIndex = lastChildIndex(d->model->index(rowCount - 1, 0, emi));
        }
        if (endIndex == -1)
            endIndex = itemIndex(emi);

        int destIndex = -1;
        if (destinationRow == d->model->rowCount(destinationParent))
        {
            const QModelIndex &emi = d->model->index(destinationRow - 1, 0, destinationParent);
            destIndex = lastChildIndex(emi) + 1;
        }
        else
        {
            destIndex = itemIndex(d->model->index(destinationRow, 0, destinationParent));
        }

        int totalMovedCount = endIndex - startIndex + 1;

        const bool visibleRowsMoved = startIndex != destIndex &&
            beginMoveRows(QModelIndex(), startIndex, endIndex, QModelIndex(), destIndex);

        const QList<TreeModelProxyPrivate::TreeItem> &buffer = d->items.mid(startIndex, totalMovedCount);
        int bufferCopyOffset;
        if (destIndex > endIndex)
        {
            for (int i = endIndex + 1; i < destIndex; i++)
            {
                d->items.move(i, i - totalMovedCount); // Fast move from 1st to 2nd position
            }
            bufferCopyOffset = destIndex - totalMovedCount;
        }
        else
        {
            // NOTE: we will not enter this loop if startIndex == destIndex
            for (int i = startIndex - 1; i >= destIndex; i--)
            {
                d->items.move(i, i + totalMovedCount); // Fast move from 1st to 2nd position
            }
            bufferCopyOffset = destIndex;
        }
        for (int i = 0; i < buffer.length(); i++) {
            TreeModelProxyPrivate::TreeItem item = buffer.at(i);
            item.depth += depthDifference;
            d->items.replace(bufferCopyOffset + i, item);
        }

        if (visibleRowsMoved)
            endMoveRows();

        if (depthDifference != 0)
        {
            const QModelIndex &topLeft = index(bufferCopyOffset, 0, QModelIndex());
            const QModelIndex &bottomRight = index(bufferCopyOffset + totalMovedCount - 1, 0, QModelIndex());
            const QVector<int> changedRole(1, TreeModelProxyPrivate::DepthRole);
            emit dataChanged(topLeft, bottomRight, changedRole);
        }
    }
}

void TreeModelProxy::modelRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    Q_D(TreeModelProxy);

    if (parent == d->rootIndex || childrenVisible(parent))
    {
        const QModelIndex &smi = d->model->index(start, 0, parent);
        int startIndex = itemIndex(smi);
        const QModelIndex &emi = d->model->index(end, 0, parent);
        int endIndex = -1;
        if (isExpanded(emi))
        {
            int rowCount = d->model->rowCount(emi);
            if (rowCount > 0)
            {
                const QModelIndex &idx = d->model->index(rowCount - 1, 0, emi);
                endIndex = lastChildIndex(idx);
            }
        }
        if (endIndex == -1)
            endIndex = itemIndex(emi);

        removeVisible(startIndex, endIndex);
    }

    for (int r = start; r <= end; r++)
    {
        const QModelIndex &cmi = d->model->index(r, 0, parent);
        d->expandedItems.remove(cmi);
    }
}

void TreeModelProxy::modelRowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_D(TreeModelProxy);

    TreeModelProxyPrivate::TreeItem item;
    int parentRow = itemIndex(parent);
    if (parentRow >= 0)
    {
        const QModelIndex& parentIndex = index(parentRow, 0);
        QVector<int> changedRole(1, TreeModelProxyPrivate::HasChildrenRole);
        emit dataChanged(parentIndex, parentIndex, changedRole);
        item = d->items.at(parentRow);
        if (!item.expanded)
            return;
    }
    else if (parent == d->rootIndex)
    {
        item = TreeModelProxyPrivate::TreeItem(parent);
    }
    else
    {
        return;
    }

    d->showChildren(item, start, end);
}

void TreeModelProxy::modelRowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
    Q_D(TreeModelProxy);

    if (childrenVisible(destinationParent))
    {
        if (!childrenVisible(sourceParent))
            modelRowsInserted(destinationParent, destinationRow, destinationRow + sourceEnd - sourceStart);
        else
        {
            int destIndex = -1;
            if (destinationRow == d->model->rowCount(destinationParent))
            {
                const QModelIndex &emi = d->model->index(destinationRow - 1, 0, destinationParent);
                destIndex = lastChildIndex(emi) + 1;
            }
            else
            {
                destIndex = itemIndex(d->model->index(destinationRow, 0, destinationParent));
            }

            const QModelIndex &emi = d->model->index(destinationRow + sourceEnd - sourceStart, 0, destinationParent);
            int endIndex = -1;
            if (isExpanded(emi))
            {
                int rowCount = d->model->rowCount(emi);
                if (rowCount > 0)
                    endIndex = lastChildIndex(d->model->index(rowCount - 1, 0, emi));
            }
            if (endIndex == -1)
                endIndex = itemIndex(emi);

            const QModelIndex &topLeft = index(destIndex, 0, QModelIndex());
            const QModelIndex &bottomRight = index(endIndex, 0, QModelIndex());
            const QVector<int> changedRole(1, TreeModelProxyPrivate::ModelIndexRole);
            emit dataChanged(topLeft, bottomRight, changedRole);
        }
    }
}

void TreeModelProxy::modelRowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(start)
    Q_UNUSED(end)
    int parentRow = itemIndex(parent);
    if (parentRow >= 0)
    {
        const QModelIndex& parentIndex = index(parentRow, 0);
        QVector<int> changedRole(1, TreeModelProxyPrivate::HasChildrenRole);
        emit dataChanged(parentIndex, parentIndex, changedRole);
    }
}
