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
#include "jsontreemodel.h"
#include "treemodel_p.h"

#include <QSet>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QUrl>
#include <QFile>
#include <QJSValue>
#include <QDebug>

class JsonTreeModelPrivate : public TreeModelPrivate
{
public:
    QJsonDocument jsonDoc;
    mutable QHash<int, QByteArray> roles;
    QSet<QString> keys;
    QString file;
};

JsonTreeModel::JsonTreeModel(QObject *parent)
    : TreeModel(*new JsonTreeModelPrivate(), parent)
{

}

JsonTreeModel::~JsonTreeModel()
{
    save();
}
QHash<int, QByteArray> JsonTreeModel::roleNames() const
{
    Q_D(const JsonTreeModel);
    return d->roles;
}

void JsonTreeModel::setRoleNames(const QHash<int, QByteArray> roleNames)
{
    Q_D(JsonTreeModel);
    d->roles.clear();
    d->roles = roleNames;

    QHash<int, QByteArray> roles = TreeModel::roleNames();
    QHash<int, QByteArray>::const_iterator it = roles.constBegin();
    while (it != roles.constEnd())
    {
        if(d->roles.contains(it.key()))
            continue;

        d->roles.insert(it.key(), it.value());
        ++it;
    }
}

void JsonTreeModel::setRoleNames(const QList<QString> &names)
{
    Q_D(JsonTreeModel);
    d->roles.clear();
    d->roles = TreeModel::roleNames();
    int i = 0;
    foreach (const QString &value, names)
    {
        d->roles.insert(Qt::UserRole + i + 1, value.toLatin1());
        ++i;
    }
}

QVariant JsonTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role > Qt::UserRole)
    {
        int column = role - Qt::UserRole - 1;
        QModelIndex modelIndex = TreeModel::index(index.row(), column, index.parent());
        return TreeModel::data(modelIndex, Qt::DisplayRole);
    }

    return TreeModel::data(index, role);
}

bool JsonTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(role > Qt::UserRole)
    {
        int column = role - Qt::UserRole - 1;
        QModelIndex modelIndex = TreeModel::index(index.row(), column, index.parent());
        return TreeModel::setData(modelIndex, value, Qt::EditRole);
    }

    return TreeModel::setData(index, value, role);
}

QVariant JsonTreeModel::json() const
{
    Q_D(const JsonTreeModel);
    return QVariant::fromValue(d->jsonDoc);
}

void JsonTreeModel::setJson(const QVariant &value)
{
    Q_D(JsonTreeModel);
    Q_UNUSED(value)
    QVariant data = value;
    if (data.userType() == qMetaTypeId<QJSValue>())
            data = data.value<QJSValue>().toVariant();

    QJsonDocument doc;
    if(data.canConvert(QVariant::Url))
    {
        QUrl url = data.toUrl();
        if(url.isLocalFile())
        {
            d->file = url.toLocalFile();
            QFile file(d->file);
            if(!file.open(QIODevice::ReadOnly))
            {
                qCritical() << file.errorString() << file.fileName();
                return;
            }
            QByteArray json = file.readAll();
            doc = QJsonDocument::fromBinaryData(json);
            if(doc.isNull())
                doc = QJsonDocument::fromJson(json);

            file.close();
        }
    }
    else
    {
         doc = QJsonDocument::fromVariant(data);
    }

    if(doc.isNull())
    {
        qCritical() << "Cannot parse json.";
        return;
    }

    if(d->jsonDoc == doc)
        return;

    clear();
    d->jsonDoc = doc;

    // dynamic role name for roleNames
    parseKeys(doc);
    setRoleNames(d->keys.toList());
    if(d->keys.size() > columnCount())
        this->insertColumns(columnCount(), d->keys.size() - columnCount());

    load();

    emit jsonChanged();
}

void JsonTreeModel::save()
{
    Q_D(JsonTreeModel);
    if(d->file.isEmpty())
    {
        return;
    }

    QJsonArray rootArray = jsonFromTree(QModelIndex());
    QJsonDocument doc = QJsonDocument(rootArray);
#ifdef QT_DEBUG
    const QByteArray data = doc.toJson(QJsonDocument::Indented);
#else
    const QByteArray data = doc.toBinaryData();
#endif
    QFile file(d->file);
    if(!file.open(QIODevice::WriteOnly))
    {
        qCritical() << file.errorString() << file.fileName();
        return;
    }
    if(file.write(data) == -1)
        qCritical() << file.errorString() << file.fileName();
    file.close();
}

void JsonTreeModel::load()
{
    Q_D(JsonTreeModel);
    if(d->jsonDoc.isNull())
        return;

    jsonToTree(d->jsonDoc);
}

QJsonArray JsonTreeModel::jsonFromTree(const QModelIndex &parent)
{
    Q_D(JsonTreeModel);
    QJsonArray jarray;
    for (int row = 0; row < rowCount(parent); ++row)
    {
        QModelIndex currentIndex = this->index(row, 0, parent);
        if(this->hasChildren(currentIndex))
        {
            // next children
            QJsonArray children = jsonFromTree(currentIndex);

            // postorder traversal
            // createa new node
            QJsonObject node;
            for (int column = 0; column < columnCount(currentIndex); ++column)
            {
                int role = Qt::UserRole + column + 1;
                QString key = d->roles.value(role);
                QVariant value = data(this->index(currentIndex.row(), column, currentIndex.parent()), role);
                if(value.isValid())
                    node.insert(key, QJsonValue::fromVariant(value));
            }
            // save old children to parent
            node.insert("children", children);
            // append current node
            jarray.append(node);
        }
        else
        {
            // create a new node
            QJsonObject node;
            for (int column = 0; column < columnCount(currentIndex); ++column)
            {
                int role = Qt::UserRole + column + 1;
                QString key = d->roles.value(role);
                QVariant value = data(this->index(currentIndex.row(), column, currentIndex.parent()), role);
                if(value.isValid())
                    node.insert(key, QJsonValue::fromVariant(value));
            }
            // save current node to parent's array
            jarray.append(node);
        }
    }

    return jarray;
}

void JsonTreeModel::parseKeys(const QJsonDocument &doc)
{
    if(doc.isNull())
        return;

    if(doc.isArray())
    {
        QJsonArray jarray = doc.array();
        parseArrayKeys(jarray);
    }
    else
    {
        QJsonObject jobject = doc.object();
        parseObjectKeys(jobject);
    }
}

void JsonTreeModel::parseObjectKeys(const QJsonObject &object)
{
    Q_D(JsonTreeModel);
    for (const QString &key : object.keys())
    {
        QJsonValue value = object.value(key);
        if(!value.isArray() && !value.isObject())
            d->keys.insert(key);
    }
}

void JsonTreeModel::parseArrayKeys(const QJsonArray &array)
{
    for (QJsonValue value : array)
    {
        if(value.isArray())
            parseArrayKeys(value.toArray());
        else if (value.isObject())
            parseObjectKeys(value.toObject());
    }
}

void JsonTreeModel::jsonToTree(const QJsonDocument &doc)
{
    if(doc.isNull())
        return;

    if(doc.isArray())
    {
        QJsonArray jarray = doc.array();
        insertArray(jarray, QModelIndex());
    }
    else
    {
        QJsonObject jobject = doc.object();
        insertObject(jobject, QModelIndex());
    }
}

void JsonTreeModel::insertObject(const QJsonObject &object, const QModelIndex &parent)
{
    Q_D(JsonTreeModel);
    int row = this->rowCount(parent);
    this->insertRow(row, parent);

    for (const QString &key : object.keys())
    {
        int role = d->roles.key(key.toLocal8Bit(), Qt::UserRole + 1);
        int column = role - Qt::UserRole - 1;
        QModelIndex currentIndex = this->index(row, column, parent);
        QJsonValue value = object.value(key);
        insertValue(value, currentIndex);
    }
}

void JsonTreeModel::insertArray(const QJsonArray &array, const QModelIndex &parent)
{
    for (const QJsonValue &value : array)
    {
        insertValue(value, parent);
    }
}

void JsonTreeModel::insertValue(const QJsonValue &value, const QModelIndex &index)
{
    if(value.isObject()) // {}
    {
        insertObject(value.toObject(), index);
    }
    else if (value.isArray()) // []
    {
        insertArray(value.toArray(), index);
    }
    else if(value.type() != QJsonValue::Undefined) // null/bool/number/string
    {
        QModelIndex child = this->index(index.row(), index.column(), index.parent());
        this->setData(child, value.toVariant(), Qt::EditRole);
    }
    else
    {
        qWarning() << "json type undefined";
    }
}
