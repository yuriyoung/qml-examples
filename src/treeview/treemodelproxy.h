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
#ifndef TREEMODELPROXY_H
#define TREEMODELPROXY_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QItemSelection>

/**
 * Because we can't implement a TreeView component.so,instead of implement a proxy model
 */
class TreeModelProxyPrivate;
class TreeModelProxy : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TreeModelProxy)
    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex RESET resetRootIndex NOTIFY rootIndexChanged)
public:
    explicit TreeModelProxy(QObject *parent = nullptr);
    ~TreeModelProxy();

    virtual QHash<int, QByteArray> roleNames() const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    QVariant model() const;
    void setModel(const QVariant &model);

    const QModelIndex &rootIndex() const;
    void setRootIndex(const QModelIndex &index);
    void resetRootIndex();

    int itemIndex(const QModelIndex &index) const;
    int lastChildIndex(const QModelIndex &index);

    void expandRow(int row);
    void collapseRow(int row);
    bool isExpanded(int row) const;

    bool isVisible(const QModelIndex &index);
    bool childrenVisible(const QModelIndex &index);
    bool removeVisible(int startIndex, int endIndex, bool destroyed = true);

    Q_INVOKABLE QItemSelection selectionRange(const QModelIndex &fromIndex, const QModelIndex &toIndex) const;
    Q_INVOKABLE QModelIndex indexOf(int row) const;
    Q_INVOKABLE bool isExpanded(const QModelIndex &index) const;
    Q_INVOKABLE void clear();

signals:
    void modelChanged();
    void expanded(const QModelIndex &index);
    void collapsed(const QModelIndex &index);
    void rootIndexChanged();

public slots:
    void expand(const QModelIndex &index);
    void collapse(const QModelIndex &index);
    QModelIndex add(const QModelIndex &parent);
    bool remove(int row, int count);

private slots:
    void modelHasBeenDestroyed();
    void modelHasBeenReset();
    void modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRigth, const QVector<int> &roles);
    void modelLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint);
    void modelLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint);
    void modelRowsAboutToBeInserted(const QModelIndex & parent, int start, int end);
    void modelRowsAboutToBeMoved(const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationRow);
    void modelRowsAboutToBeRemoved(const QModelIndex & parent, int start, int end);
    void modelRowsMoved(const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationRow);
    void modelRowsInserted(const QModelIndex & parent, int start, int end);
    void modelRowsRemoved(const QModelIndex & parent, int start, int end);

protected:
    TreeModelProxy(TreeModelProxyPrivate &dd, QObject *parent = nullptr);
    QScopedPointer<TreeModelProxyPrivate> d_ptr;
};

#endif // TREEMODELPROXY_H
