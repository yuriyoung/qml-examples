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
 * @filename tablemodel.cpp
 * @class TableModel
 * @author Yuri Young<yuri.young@qq.com>
 * @qq 12319597
 */

#include "tablemodel.h"
#include "sql.h"

#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlIndex>
#include <QDateTime>
#include <QItemSelectionModel>
#include <QUrl>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcTableModel, "app.TableModel")

class TableModelPrivate
{
    Q_DECLARE_PUBLIC(TableModel)
public:

    void handleDatanaseChanged();
    void handleTableChanged();

    QString databaseName;
    QString tableName;
    QString errorString;
    bool completed = false;
    QItemSelectionModel *selectionModel = nullptr;
    mutable QHash<int, QByteArray> roles;
    TableModel *q_ptr = nullptr;
};

void TableModelPrivate::handleDatanaseChanged()
{
    Q_Q(TableModel);
    q->database().setDatabaseName(databaseName);
    q->database().open();
}

void TableModelPrivate::handleTableChanged()
{

}

/**
 * @brief TableModel::TableModel
 * @param parent
 */
TableModel::TableModel(QObject *parent)
    : QSqlRelationalTableModel(parent, Sql::database())
    , d_ptr(new TableModelPrivate())
{
    Q_D(TableModel);
    d->q_ptr = this;

    setEditStrategy(OnFieldChange);
}

TableModel::~TableModel()
{

}

void TableModel::classBegin()
{
    qDebug(lcTableModel) << "TableModel::classBegin";
}

void TableModel::componentComplete()
{
    Q_D(TableModel);
    qDebug(lcTableModel) << "TableModel::componentComplete";
    d->completed = true;

    // connect signals slots
    // ...

    this->database().close();
    this->database().setDatabaseName(d->databaseName);
    this->database().open();
    QSqlRelationalTableModel::setTable(d->tableName);

    qDebug() << "database:" << this->database().databaseName()
             << ", table:"  << this->tableName()
             << ", connection name:"  << this->database().connectionName();

    this->select();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    Q_D(const TableModel);
    QHash<int, QByteArray> roles;

    // for checked
    roles.insert(Qt::CheckStateRole, QByteArrayLiteral("checkState"));

    // database table fileds
    QSqlRecord record = this->record();
    for (int i = 0; i < record.count(); ++i)
    {
        roles.insert(Qt::UserRole + 1 + i, record.fieldName(i).toUtf8());
    }

    d->roles = roles;

    return roles;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(TableModel);
    if(!index.isValid())
        return false;

    if(role < Qt::UserRole)
    {
        if(role == Qt::CheckStateRole)
        {
            QItemSelectionModel::SelectionFlag flag = value.toBool()
                    ? QItemSelectionModel::Select
                    : QItemSelectionModel::Deselect;
            d->selectionModel->select(index, flag);
            emit selectionChanged();
            emit dataChanged(index, index, {role});
            return true;
        }

        return QSqlRelationalTableModel::setData(index, value, role);
    }

    int column = role - Qt::UserRole - 1;
    QModelIndex modelIndex = createIndex(index.row(), column);
    return QSqlRelationalTableModel::setData(modelIndex, value, Qt::EditRole);
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    Q_D(const TableModel);
    if (!index.isValid())
        return QVariant();

    QVariant value;
    if(role < Qt::UserRole)
    {
        if(role == Qt::CheckStateRole)
            return d->selectionModel->isSelected(index);

        return QSqlRelationalTableModel::data(index, role);
    }

    int column = role - Qt::UserRole - 1;
    QModelIndex modelIndex = createIndex(index.row(), column);
    return QSqlRelationalTableModel::data(modelIndex, Qt::DisplayRole);
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
     Q_D(TableModel);
    if (parent.isValid() || row < 0 || count <= 0)
        return false;
    else if (row + count > rowCount())
        return false;
    else if (!count)
        return true;

    bool success = false;
    int softDeleteColumn = fieldIndex("deleted_at");
    bool supportSoftDelete = softDeleteColumn != -1;
    if(supportSoftDelete)
    {
        for (int idx = row + count - 1; idx >= row; --idx)
        {
            QString value = data(createIndex(row, softDeleteColumn)).toString();
            QDateTime dt = QDateTime::fromString(value, Qt::ISODate);
            if(dt.isValid())
            {
                // hard delete
                success = QSqlRelationalTableModel::removeRows(idx, 1, parent);
            }
            else
            {
                // soft delete (just filling deleted_at field)
                QString t = QDateTime::currentDateTime().toString(Qt::ISODate);
                success = setData(createIndex(idx, softDeleteColumn), t, Qt::EditRole);
            }

            if(!success)
            {
                d->errorString =  "Something wrong happen " + this->lastError().text();
                qWarning() << d->errorString;
                emit error(d->errorString);
                break;
            }
        }

        if(success && editStrategy() == OnManualSubmit)
            return submitAll();
    }
    else
    {
        success = QSqlRelationalTableModel::removeRows(row, count, parent);
    }

    return success;
}

void TableModel::setDatabaseName(const QString &fileName)
{
    Q_D(TableModel);
    if(!fileName.compare(d->databaseName, Qt::CaseInsensitive))
        return;

    if(d->completed)
    {
        this->database().close();
        this->database().setDatabaseName(fileName);
        this->database().open();
    }

    d->databaseName = fileName;
    emit databaseNameChanged();
}

QString TableModel::databaseName() const
{
    Q_D(const TableModel);
    return d->databaseName;
}

void TableModel::setTable(const QString &tableName)
{
    Q_D(TableModel);
    // remove whitespace from the start and the end
    const QString table = tableName.trimmed();
    if(!table.compare(this->tableName(), Qt::CaseInsensitive))
        return;

    if(this->database().isValid() && d->completed)
    {
        QSqlRelationalTableModel::setTable(table);
    }

    d->tableName = table;
    emit tableChanged();
}

QString TableModel::tableName() const
{
    return QSqlRelationalTableModel::tableName();
}

int TableModel::selectedRows() const
{
    Q_D(const TableModel);
    if(!d->selectionModel)
        return 0;

    return d->selectionModel->selectedIndexes().count();
}

QString TableModel::errorString() const
{
    Q_D(const TableModel);
    return d->errorString.isEmpty() ? this->lastError().text() : d->errorString;
}

bool TableModel::select()
{
    return this->refresh();
}

bool TableModel::refresh()
{
    Q_D(TableModel);
    if(!this->database().tables().contains(this->tableName()))
    {
        QString msg = QString("Can not open table '%1' in '%2'")
                .arg(this->tableName(), this->database().databaseName());
        qWarning(lcTableModel) << msg;
        d->errorString = msg;
        emit error(msg);
        return false;
    }

    bool ok = QSqlRelationalTableModel::select();
    if(!ok)
    {
        QString msg = "Read record error " + this->lastError().text();
        qWarning(lcTableModel) << msg;
        d->errorString = msg;
        emit error(msg);
    }

    return ok;
}

int TableModel::add()
{
    return this->insert(this->rowCount());
}

int TableModel::insert(int row)
{
    Q_D(TableModel);
    QSqlRecord rec = this->record();
    for (int i = 0; i < rec.count(); ++i)
        rec.setGenerated(i, false);

    rec.setValue("state", TableModel::PendingStatus);
    rec.setGenerated("state", true);

    bool ok = this->insertRecord(row, rec);
    if (!ok)
    {
        d->errorString += "";
        d->errorString += "Insert record failed" + this->lastError().text();
        d->errorString += this->databaseName() + this->tableName();
        qDebug(lcTableModel) << d->errorString;
        emit error(d->errorString);
        return -1;
    }

    return row;
}

bool TableModel::remove(int row)
{
//    Q_D(TableModel);
//    int role = d->roles.key(QByteArrayLiteral("state"));
//    int val = data(index(row, 0), role).toInt();
//    if(val == PendingStatus)
//        return false;

    return this->removeRow(row);
}

int TableModel::removeSelected()
{
    Q_D(TableModel);
    int total = 0;

    QModelIndexList list = d->selectionModel->selectedIndexes();
    typedef QList<QModelIndex>::iterator ModelIterator;
    ModelIterator begin = list.begin();
    ModelIterator end = list.end();
    std::sort(begin, end);

    while (!list.isEmpty())
    {
        QModelIndex last = list.takeLast();
        int currentRow = last.row();

        if(removeRow(currentRow))
        {
            d->selectionModel->select(last, QItemSelectionModel::Deselect);
            ++total;
        }
    }

    return total;
}

bool TableModel::recoverRow(int row)
{
    Q_D(TableModel);
    QModelIndex modelIndex = createIndex(row, 0);
    int role = d->roles.key("deleted_at");
    return this->setData(modelIndex, QVariant(), role);
}

int TableModel::recoverSelected()
{
    Q_D(TableModel);
    int total = 0;

    QModelIndexList list = d->selectionModel->selectedIndexes();
    typedef QList<QModelIndex>::iterator ModelIterator;
    ModelIterator begin = list.begin();
    ModelIterator end = list.end();
    std::sort(begin, end);

    while (!list.isEmpty())
    {
        QModelIndex last = list.takeLast();
        if(this->recoverRow(last.row()))
        {
            d->selectionModel->select(last, QItemSelectionModel::Deselect);
            ++total;
        }
    }

    return total;
}
