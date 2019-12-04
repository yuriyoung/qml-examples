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
#ifndef JSONTREEMODEL_H
#define JSONTREEMODEL_H

#include "treemodel.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QVariant>
#include <QHash>

class JsonTreeModelPrivate;
class JsonTreeModel : public TreeModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(JsonTreeModel)
    Q_PROPERTY(QVariant json READ json WRITE setJson NOTIFY jsonChanged)
public:
    explicit JsonTreeModel(QObject *parent = nullptr);
    ~JsonTreeModel();

    QHash<int, QByteArray> roleNames() const;
    void setRoleNames(const QHash<int, QByteArray> roleNames);
    void setRoleNames(const QList<QString> &names);

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant json() const;
    void setJson(const QVariant &value);

signals:
    void jsonChanged();

public slots:
    void save();
    void load();

protected:
    QJsonArray jsonFromTree(const QModelIndex &parent = QModelIndex());

    void parseKeys(const QJsonDocument &doc);
    void parseObjectKeys(const QJsonObject &object);
    void parseArrayKeys(const QJsonArray &array);

    void jsonToTree(const QJsonDocument &doc);
    void insertObject(const QJsonObject &object, const QModelIndex &parent = QModelIndex());
    void insertArray(const QJsonArray &array, const QModelIndex &parent = QModelIndex());
    void insertValue(const QJsonValue &value, const QModelIndex &index);
};

#endif // JSONTREEMODEL_H
