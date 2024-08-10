/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.16)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../include/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.16. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata0[395];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 8), // "openFile"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 10), // "flag_start"
QT_MOC_LITERAL(4, 32, 12), // "openfile_txt"
QT_MOC_LITERAL(5, 45, 38), // "std::vector<std::pair<double,..."
QT_MOC_LITERAL(6, 84, 4), // "path"
QT_MOC_LITERAL(7, 89, 16), // "openfile_GeoJson"
QT_MOC_LITERAL(8, 106, 45), // "std::map<QString,std::pair<QS..."
QT_MOC_LITERAL(9, 152, 25), // "openfile_insertpoints_txt"
QT_MOC_LITERAL(10, 178, 39), // "std::vector<std::pair<double,..."
QT_MOC_LITERAL(11, 218, 4), // "vpts"
QT_MOC_LITERAL(12, 223, 27), // "openfile_insertpoints_gjson"
QT_MOC_LITERAL(13, 251, 46), // "std::map<QString,std::pair<QS..."
QT_MOC_LITERAL(14, 298, 24), // "openfile_insertedges_txt"
QT_MOC_LITERAL(15, 323, 4), // "vegs"
QT_MOC_LITERAL(16, 328, 26), // "openfile_insertedges_gjson"
QT_MOC_LITERAL(17, 355, 9), // "isGeoJson"
QT_MOC_LITERAL(18, 365, 18), // "save_to_file_paths"
QT_MOC_LITERAL(19, 384, 10) // "save_files"

    },
    "MainWindow\0openFile\0\0flag_start\0"
    "openfile_txt\0std::vector<std::pair<double,double> >\0"
    "path\0openfile_GeoJson\0"
    "std::map<QString,std::pair<QString,QString> >\0"
    "openfile_insertpoints_txt\0"
    "std::vector<std::pair<double,double> >&\0"
    "vpts\0openfile_insertpoints_gjson\0"
    "std::map<QString,std::pair<QString,QString> >&\0"
    "openfile_insertedges_txt\0vegs\0"
    "openfile_insertedges_gjson\0isGeoJson\0"
    "save_to_file_paths\0save_files"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       4,    1,   67,    2, 0x0a /* Public */,
       7,    1,   70,    2, 0x0a /* Public */,
       9,    1,   73,    2, 0x0a /* Public */,
      12,    1,   76,    2, 0x0a /* Public */,
      14,    2,   79,    2, 0x0a /* Public */,
      16,    2,   84,    2, 0x0a /* Public */,
      17,    1,   89,    2, 0x0a /* Public */,
      18,    2,   92,    2, 0x0a /* Public */,
      19,    0,   97,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    0x80000000 | 5, QMetaType::QString,    6,
    0x80000000 | 8, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   11,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 10,   15,   11,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 13,   15,   11,
    QMetaType::Bool, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->openFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: { std::vector<std::pair<double,double> > _r = _t->openfile_txt((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::vector<std::pair<double,double> >*>(_a[0]) = std::move(_r); }  break;
        case 2: { std::map<QString,std::pair<QString,QString> > _r = _t->openfile_GeoJson((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::map<QString,std::pair<QString,QString> >*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->openfile_insertpoints_txt((*reinterpret_cast< std::vector<std::pair<double,double> >(*)>(_a[1]))); break;
        case 4: _t->openfile_insertpoints_gjson((*reinterpret_cast< std::map<QString,std::pair<QString,QString> >(*)>(_a[1]))); break;
        case 5: _t->openfile_insertedges_txt((*reinterpret_cast< std::vector<std::pair<double,double> >(*)>(_a[1])),(*reinterpret_cast< std::vector<std::pair<double,double> >(*)>(_a[2]))); break;
        case 6: _t->openfile_insertedges_gjson((*reinterpret_cast< std::map<QString,std::pair<QString,QString> >(*)>(_a[1])),(*reinterpret_cast< std::map<QString,std::pair<QString,QString> >(*)>(_a[2]))); break;
        case 7: { bool _r = _t->isGeoJson((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->save_to_file_paths((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->save_files(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
