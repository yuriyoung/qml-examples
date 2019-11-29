/****************************************************************************
** Meta object code from reading C++ file 'tablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/tableview/tablemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TableModel_t {
    QByteArrayData data[27];
    char stringdata0[292];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TableModel_t qt_meta_stringdata_TableModel = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TableModel"
QT_MOC_LITERAL(1, 11, 19), // "databaseNameChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "tableChanged"
QT_MOC_LITERAL(4, 45, 19), // "selectedRowsChanged"
QT_MOC_LITERAL(5, 65, 5), // "error"
QT_MOC_LITERAL(6, 71, 7), // "message"
QT_MOC_LITERAL(7, 79, 6), // "select"
QT_MOC_LITERAL(8, 86, 7), // "refresh"
QT_MOC_LITERAL(9, 94, 3), // "add"
QT_MOC_LITERAL(10, 98, 6), // "insert"
QT_MOC_LITERAL(11, 105, 3), // "row"
QT_MOC_LITERAL(12, 109, 6), // "remove"
QT_MOC_LITERAL(13, 116, 14), // "removeSelected"
QT_MOC_LITERAL(14, 131, 10), // "recoverRow"
QT_MOC_LITERAL(15, 142, 15), // "recoverSelected"
QT_MOC_LITERAL(16, 158, 8), // "database"
QT_MOC_LITERAL(17, 167, 5), // "table"
QT_MOC_LITERAL(18, 173, 12), // "selectedRows"
QT_MOC_LITERAL(19, 186, 11), // "errorString"
QT_MOC_LITERAL(20, 198, 10), // "ItemStatus"
QT_MOC_LITERAL(21, 209, 11), // "SavedStatus"
QT_MOC_LITERAL(22, 221, 13), // "StickedStatus"
QT_MOC_LITERAL(23, 235, 16), // "DuplicatedStatus"
QT_MOC_LITERAL(24, 252, 13), // "PendingStatus"
QT_MOC_LITERAL(25, 266, 11), // "ErrorStatus"
QT_MOC_LITERAL(26, 278, 13) // "DeletedStatus"

    },
    "TableModel\0databaseNameChanged\0\0"
    "tableChanged\0selectedRowsChanged\0error\0"
    "message\0select\0refresh\0add\0insert\0row\0"
    "remove\0removeSelected\0recoverRow\0"
    "recoverSelected\0database\0table\0"
    "selectedRows\0errorString\0ItemStatus\0"
    "SavedStatus\0StickedStatus\0DuplicatedStatus\0"
    "PendingStatus\0ErrorStatus\0DeletedStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TableModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       4,   94, // properties
       1,  110, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    1,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   80,    2, 0x0a /* Public */,
       8,    0,   81,    2, 0x0a /* Public */,
       9,    0,   82,    2, 0x0a /* Public */,
      10,    1,   83,    2, 0x0a /* Public */,
      12,    1,   86,    2, 0x0a /* Public */,
      13,    0,   89,    2, 0x0a /* Public */,
      14,    1,   90,    2, 0x0a /* Public */,
      15,    0,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   11,
    QMetaType::Bool, QMetaType::Int,   11,
    QMetaType::Int,
    QMetaType::Bool, QMetaType::Int,   11,
    QMetaType::Int,

 // properties: name, type, flags
      16, QMetaType::QString, 0x00495003,
      17, QMetaType::QString, 0x00495103,
      18, QMetaType::QString, 0x00495001,
      19, QMetaType::QString, 0x00095001,

 // properties: notify_signal_id
       0,
       1,
       2,
       0,

 // enums: name, alias, flags, count, data
      20,   20, 0x0,    6,  115,

 // enum data: key, value
      21, uint(TableModel::SavedStatus),
      22, uint(TableModel::StickedStatus),
      23, uint(TableModel::DuplicatedStatus),
      24, uint(TableModel::PendingStatus),
      25, uint(TableModel::ErrorStatus),
      26, uint(TableModel::DeletedStatus),

       0        // eod
};

void TableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TableModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->databaseNameChanged(); break;
        case 1: _t->tableChanged(); break;
        case 2: _t->selectedRowsChanged(); break;
        case 3: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: { bool _r = _t->select();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->refresh();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->add();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->insert((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->remove((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->removeSelected();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->recoverRow((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->recoverSelected();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TableModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableModel::databaseNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TableModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableModel::tableChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TableModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableModel::selectedRowsChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TableModel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableModel::error)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<TableModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->databaseName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->tableName(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->selectedRows(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<TableModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDatabaseName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setTable(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject TableModel::staticMetaObject = { {
    &QSqlRelationalTableModel::staticMetaObject,
    qt_meta_stringdata_TableModel.data,
    qt_meta_data_TableModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TableModel.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QQmlParserStatus"))
        return static_cast< QQmlParserStatus*>(this);
    return QSqlRelationalTableModel::qt_metacast(_clname);
}

int TableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlRelationalTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TableModel::databaseNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TableModel::tableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TableModel::selectedRowsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TableModel::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
