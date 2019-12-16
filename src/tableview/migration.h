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

#ifndef MIGRATION_H
#define MIGRATION_H

#include <QObject>

class QSqlDatabase;
class MigrationPrivate;
class Migration
{
    Q_DECLARE_PRIVATE(Migration)
    Q_DISABLE_COPY(Migration)
public:
    explicit Migration();
    explicit Migration(const QSqlDatabase &db);
    virtual ~Migration();

    void setConnection(const QSqlDatabase &db);
    QSqlDatabase connection() const;
    QStringList files() const;

    virtual bool run(const QStringList &files);
    virtual bool run(const QString &path);
    virtual bool reset(const QStringList &files);
    virtual bool reset(const QString &path);

protected:
    virtual QString table() const;
    virtual bool migrateUp(const QString &file);
    virtual bool migrateDown(const QString &file);
    virtual bool runMigration(const QStringList &files);
    virtual bool rollbackMigration(const QStringList &migrations, const QStringList &files);

    bool toRepository(const QString &migration);
    bool takeRepository(const QString &migration);
    bool createRepository();
    bool repositoryExists();
    int lastBatchNumber();

private:
    QScopedPointer<MigrationPrivate> d_ptr;
};

#endif // MIGRATION_H
