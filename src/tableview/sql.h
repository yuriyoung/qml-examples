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
 * @filename sql.h
 * @class Sql
 * @author Yuri Young<yuri.young@qq.com>
 * @qq 12319597
 */
#ifndef SQL_H
#define SQL_H

#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QThreadStorage>
#include <QUuid>
#include <QDebug>

const QString DRIVER = "QSQLITE";
static QThreadStorage<QSqlDatabase> databasePool;

class Sql
{
public:
    static QSqlDatabase database(const QString &databaseName = QString(), bool open = true)
    {
        QSqlDatabase db;

        if(!databasePool.hasLocalData())
        {
            QString connName = QUuid::createUuid().toString(QUuid::Id128);
            db = QSqlDatabase::addDatabase(DRIVER, connName);
            if(!databaseName.isEmpty())
                db.setDatabaseName(databaseName);
            if(open)
                db = QSqlDatabase::database(connName, open);
            databasePool.setLocalData(db);
        }
        db = databasePool.localData();
        return db;
    }

    static QSqlDatabase connection(const QString &connectionName)
    {
        if(connectionName.isEmpty())
           return QSqlDatabase::database(QSqlDatabase::defaultConnection, true);

        return QSqlDatabase::database(connectionName, true);
    }

    static QSqlDatabase connection()
    {
        QString connectionName = QUuid::createUuid().toString(QUuid::Id128);
        return QSqlDatabase::database(connectionName, true);
    }
};

#endif // SQL_H
