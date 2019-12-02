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
 *
 * @date 2019/11/28-11-28
 * @filename tablemodel.h
 * @class TableModel
 * @author Yuri Young<yuri.young@qq.com>
 * @qq 12319597
 */
#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QSqlRelationalTableModel>
#include <QQmlParserStatus>

class TableModelPrivate;
class TableModel : public QSqlRelationalTableModel,  public QQmlParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TableModel)
    QScopedPointer<TableModelPrivate> d_ptr;
    Q_PROPERTY(QString database READ databaseName WRITE setDatabaseName NOTIFY databaseNameChanged)
    Q_PROPERTY(QString table READ tableName WRITE setTable NOTIFY tableChanged)
    Q_PROPERTY(QString selectedRows READ selectedRows NOTIFY selectionChanged)
    Q_PROPERTY(QString errorString READ errorString)
    Q_ENUMS(ItemStatus)
public:
    enum ItemStatus {
        SavedStatus = 0,
        StickedStatus,
        DuplicatedStatus,
        PendingStatus,
        ErrorStatus,
        DeletedStatus
    };

    explicit TableModel(QObject *parent = nullptr);
    ~TableModel() override;

    void classBegin() override;
    void componentComplete() override;

    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setDatabaseName(const QString &fileName);
    QString databaseName() const;

    void setTable(const QString &tableName) override;
    QString tableName() const;

    int selectedRows() const;

    QString errorString() const;

signals:
    void databaseNameChanged();
    void tableChanged();
    void selectionChanged();
    void error(const QString &message);

public slots:
    bool select() override;
    virtual bool refresh();

    int add();
    int insert(int row);
    bool remove(int row);
    int removeSelected();
    bool recoverRow(int row);
    int recoverSelected();
};

#endif // TABLEMODEL_H
