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

#include <QAbstractListModel>

/**
 * Because we can't implement a TreeView component.so,instead of implement a proxy model
 */
class TreeModelProxyPrivate;
class TreeModelProxy : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TreeModelProxy)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex RESET resetRootIndex NOTIFY rootIndexChanged)
public:
    explicit TreeModelProxy(QObject *parent = nullptr);
    ~TreeModelProxy();

    virtual QHash<int, QByteArray> roleNames() const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);
//    void setModel(const QVariant &model); // forget it

signals:
    void modelChanged();
    void expanded(const QModelIndex &index);
    void collapsed(const QModelIndex &index);

public slots:
    void expand(const QModelIndex &index);
    void collapse(const QModelIndex &index);

protected:
    TreeModelProxy(TreeModelProxyPrivate &dd, QObject *parent = nullptr);
    QScopedPointer<TreeModelProxyPrivate> const d_ptr;
};

#endif // TREEMODELPROXY_H
