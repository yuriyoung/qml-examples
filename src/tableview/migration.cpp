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
 */

/*
* @date 2019/12/4-12-4
* @filename migration.cpp
* @class Migration
* @author Yuri Young<yuri.young@qq.com>
* @qq 12319597
*/

#include "migration.h"

#include <QSet>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcMigration, "app.Migration")

class MigrationPrivate
{
    Q_DECLARE_PUBLIC(Migration)
public:
    bool migrationExists(const QString &name);
    QStringList migrations();
    void pendingMigration(const QStringList &files);
    QStringList migrationFiles(const QString &path);
    QString migrationName(const QString &file);
    QStringList resolveStatements(const QString &file);
    QString resolveInstance(const QString &fileName);

    int lastBatch = 0;
    QSet<QString> files;
    QSqlDatabase connection;
    Migration *q_ptr = nullptr;
};

bool MigrationPrivate::migrationExists(const QString &name)
{
    const QString instanceName = this->resolveInstance(name);
    bool exists = connection.tables().contains(instanceName);
//    if(exists)
//    {
        // migration respository dose not exists? --never mine
//        this->toRepository(name);
//    }

    return exists;
}

QStringList MigrationPrivate::migrations()
{
    Q_Q(Migration);
    QStringList migrations;
    QSqlQuery query(connection);
    if(!query.exec(QString("SELECT DISTINCT migration FROM %1").arg(q->table())))
    {
        qCritical(lcMigration()) << "Migration error:" << query.lastError().text();
        return migrations;
    }

    while (query.next())
    {
        migrations << query.value(0).toString();
    }

    return migrations;
}

/**
 * @brief get the migration files that have not run yet
 * @param files
 */
void MigrationPrivate::pendingMigration(const QStringList &files)
{
    this->files.clear();
    for(const QString &file : files)
    {
        if(!file.endsWith(".sql"))
            continue;

        QString name = this->migrationName(file);
        if(this->resolveInstance(name).isEmpty())
            continue;

        this->files.insert(file);
    }
}

/**
 * @brief get all migration files from the path
 * @param path
 * @return
 */
QStringList MigrationPrivate::migrationFiles(const QString &path)
{
    QDir dir(path);
    QStringList files;

    // sort by name
    QStringList names = dir.entryList({"*.sql"}, QDir::Files, QDir::Name);
    foreach(const QString &name, names)
    {
        QString fileName = QDir::cleanPath(path + QDir::separator() + name);
        files << QDir::toNativeSeparators(fileName);
    }

    return files;
}

/**
 * @brief get migration file name without extension
 * @param file
 * @return
 */
QString MigrationPrivate::migrationName(const QString &file)
{
    return QFileInfo(file).baseName(); // without extension
}

/**
 * @brief resolve a migrationn sql statements from a file
 * @param file
 * @return
 */
QStringList MigrationPrivate::resolveStatements(const QString &file)
{
    QStringList statements;
    QFile sqlFile(file);
    if(!sqlFile.open(QIODevice::ReadOnly))
    {
        qCritical(lcMigration) << "Can not open file '" << file << "' " << sqlFile.errorString();
        return statements;
    }

    const QString sql = sqlFile.readAll().trimmed();
    sqlFile.close();

    statements = sql.split(QRegExp(";"), QString::SkipEmptyParts);
//    if(statements.last() == "\n" || statements.last() == "\r\n")
//        statements.removeLast();

    return statements;
}

/**
 * @brief resolve a migrationn table instance from a file
 * @param fileName
 * @return
 */
QString MigrationPrivate::resolveInstance(const QString &fileName)
{
    QString instance;
    QString name = this->migrationName(fileName);

    QStringList pieces = name.split("_");
    if(pieces.count() < 2)
        return instance;
    /*
    if (pieces.count() % 2 == 0) // 001_Instances.sql / 001_Instances_Add_Fileds.sql
        instance = pieces[1];
    else if (pieces.count() % 2 == 1) // 001_Instance_Relation.sql / 001_Instance_Relation_Add_Fields.sql
        instance = pieces[1] + "_" + pieces[2];
    return instance.toLower();
    */

    pieces.removeFirst();
    return pieces.join("_");
}


/**
 * @brief Migration::Migration
 */
Migration::Migration()
    : d_ptr(new MigrationPrivate())
{
    d_ptr->q_ptr = this;
}

Migration::Migration(const QSqlDatabase &db)
    : d_ptr(new MigrationPrivate())
{
    d_ptr->connection = db;
    d_ptr->q_ptr = this;
}

Migration::~Migration()
{
    Q_D(Migration);
    d->files.clear();
}

void Migration::setConnection(const QSqlDatabase &db)
{
    Q_D(Migration);
    d->connection = db;
}

QSqlDatabase Migration::connection() const
{
    Q_D(const Migration);
    return d->connection;
}

QStringList Migration::files() const
{
    Q_D(const Migration);
    return d->files.toList();
}

/**
 * @brief do migratetion the specified files
 * @param files
 * @return
 */
bool Migration::run(const QStringList &files)
{
    Q_D(Migration);
    if(!d->connection.isValid())
    {
        qWarning(lcMigration) << "Invalid connection.";
        return false;
    }

    if(!d->connection.isOpen())
        d->connection.open();

    // initialize migration repository
    this->createRepository();

    d->pendingMigration(files);
    if(d->files.isEmpty())
    {
        qWarning(lcMigration) << "Nothing to migrate.";
        return false;
    }

    return this->runMigration(d->files.toList());
}

/**
 * @brief do migratetion the files from the specified path
 * @param path
 * @return
 */
bool Migration::run(const QString &path)
{
    Q_D(Migration);
    if(!d->connection.isValid())
    {
        qWarning(lcMigration) << "Invalid connection.";
        return false;
    }

    if(!d->connection.isOpen())
        d->connection.open();

    // initialize migration repository
    this->createRepository();

    QStringList files = d->migrationFiles(path);
    d->pendingMigration(files);
    if(d->files.isEmpty())
    {
        qWarning(lcMigration) << "Nothing to migrate.";
        return false;
    }

    return this->runMigration(d->files.toList());
}

/**
 * @brief rolls all of the currently applied migrations back.
 * @param files
 * @return
 */
bool Migration::reset(const QStringList &files)
{
    Q_D(Migration);
    QStringList names = d->migrations();
    if(names.isEmpty())
    {
        qDebug(lcMigration) << "Nothing to rollback";
        return false;
    }

    if(!d->connection.isValid())
        return false;

    if(!d->connection.isOpen())
        d->connection.open();

    return this->rollbackMigration(names, files);
}

/**
 * @brief rolls all of the currently applied migrations back.
 * @param path
 * @return
 */
bool Migration::reset(const QString &path)
{
    Q_D(Migration);
    QStringList files = d->migrationFiles(path);
    QStringList names = d->migrations();
    if(names.isEmpty())
    {
        qDebug(lcMigration) << "Nothing to rollback";
        return false;
    }

    if(!d->connection.isValid())
        return false;

    if(!d->connection.isOpen())
        d->connection.open();

    return this->rollbackMigration(names, files);
}

QString Migration::table() const
{
    return "migrations";
}

/**
 * @brief migrate up tables (create tables)
 * @param file
 * @return
 */
bool Migration::migrateUp(const QString &file)
{
    Q_D(Migration);
    const QStringList statements = d->resolveStatements(file);
    bool transacted = d->connection.driver()->hasFeature(QSqlDriver::Transactions);

    if(transacted)
        d->connection.transaction();
    foreach(const QString &cmd, statements)
    {
        d->connection.exec(cmd);
        if(d->connection.lastError().type() != QSqlError::NoError)
        {
            qCritical(lcMigration) << "Migration up error '" << file << "' " << d->connection.lastError().text();
            qCritical(lcMigration) << cmd;
            if(transacted)
                d->connection.rollback();
            return false;
        }
    }
    if(transacted)
        d->connection.commit();

    return true;
}

/**
 * @brief migrate down tables by the specfied file (drop tables)
 * @param file
 * @return
 */
bool Migration::migrateDown(const QString &file)
{
    Q_D(Migration);
    QString tableName = d->resolveInstance(file);

    bool transacted = d->connection.driver()->hasFeature(QSqlDriver::Transactions);
    QStringList tables = d->connection.tables();

    if(transacted)
    {
        d->connection.transaction();
    }
//    foreach(const QString &table, tables)
//    {
    const QString cmd = QString("DROP TABLE %1").arg(tableName);
    d->connection.exec(cmd);
    if(d->connection.lastError().type() != QSqlError::NoError)
    {
        qCritical(lcMigration) << "Migration down error" << d->connection.lastError().text();
        if(transacted)
            d->connection.rollback();
        return false;
    }
//    }
    if(transacted)
    {
        d->connection.commit();
    }
    return true;
}

/**
 * @brief start migration
 * @param files
 * @return
 */
bool Migration::runMigration(const QStringList &files)
{
    Q_D(Migration);
    foreach(const QString &file, files)
    {
        // check file has been migrated
        QString name = d->migrationName(file);
        if(d->migrationExists(name))
            continue;

        if(!this->migrateUp(file))
        {
            return false;
        }

        if(!this->toRepository(name))
            continue;
    }
    d->files.clear();

    return true;
}

/**
 * @brief rollback migrations (start drop table)
 * @param migrations
 * @param files
 * @return
 */
bool Migration::rollbackMigration(const QStringList &migrations, const QStringList &files)
{
    Q_D(Migration);
    bool ok = false;
    foreach(const QString &file, files)
    {
        QString name = d->migrationName(file);
        if(!migrations.contains(name))
        {
            qWarning(lcMigration) << "Migration not found:" << name;
            continue;
        }

        if(!this->migrateDown(file))
            return false;

        ok = this->takeRepository(name);
    }

    return ok;
}

/**
 * @brief save file name to migration repository
 * @param migration
 * @return bool
 */
bool Migration::toRepository(const QString &migration)
{
    Q_D(Migration);
    d->lastBatch = this->lastBatchNumber() + 1;
    // insert if dose not exist (otherwise update it 'REPLACE' )
    QString sql = QString("INSERT INTO %1 (migration, batch) VALUES (?, ?)").arg(table());
    QSqlQuery query(d->connection);
    query.prepare(sql);
    query.addBindValue(migration);
    query.addBindValue(d->lastBatch);
    if(!query.exec())
    {
        qCritical(lcMigration) << "Migration error:" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief remove migration repository
 * @param migration
 * @return bool
 */
bool Migration::takeRepository(const QString &migration)
{
    Q_D(Migration);
    d->lastBatch = this->lastBatchNumber() + 1;
    QString sql = QString("DELETE FROM %1 WHERE migration='%2'").arg(table(), migration);
    QSqlQuery query(d->connection);
    if(!query.exec(sql))
    {
        qWarning(lcMigration) << "Rollback migration waring:" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief create migration repository if dose not exists, otherwise do nothing
 * @return bool
 */
bool Migration::createRepository()
{
    Q_D(Migration);
    if(!this->repositoryExists())
    {
        d->connection.exec(QString("CREATE TABLE %1 ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "migration VARCHAR(255) NOT NULL DEFAULT '',"
                           "batch INTEGER NOT NULL DEFAULT 1)").arg(table()));
        if(d->connection.lastError().type() != QSqlError::NoError)
            return false;
    }

    return true;
}

bool Migration::repositoryExists()
{
    Q_D(Migration);
    return d->connection.tables().contains(table());
}

/**
 * @brief get last migration veriosn number
 * @return int
 */
int Migration::lastBatchNumber()
{
    Q_D(Migration);
    QSqlQuery query(d->connection);
    if(!query.exec(QString("SELECT MAX(batch) AS batch FROM %1").arg(table())))
    {
        qCritical(lcMigration) << "Migration error:" << query.lastError().text();
        return 0;
    }

    if(!query.next())
        return 0;

    return query.value("batch").toInt();
}
