/****************************************************************************
** Meta object code from reading C++ file 'flowermodeling.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../flowermodeling.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flowermodeling.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_flowermodeling_t {
    QByteArrayData data[15];
    char stringdata0[323];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_flowermodeling_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_flowermodeling_t qt_meta_stringdata_flowermodeling = {
    {
QT_MOC_LITERAL(0, 0, 14), // "flowermodeling"
QT_MOC_LITERAL(1, 15, 22), // "on_Button_scan_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 24), // "on_Button_center_clicked"
QT_MOC_LITERAL(4, 64, 23), // "on_Button_paint_clicked"
QT_MOC_LITERAL(5, 88, 22), // "on_Button_done_clicked"
QT_MOC_LITERAL(6, 111, 12), // "SetBrushsize"
QT_MOC_LITERAL(7, 124, 26), // "on_Button_template_clicked"
QT_MOC_LITERAL(8, 151, 25), // "on_Button_surface_clicked"
QT_MOC_LITERAL(9, 177, 22), // "on_Button_mesh_clicked"
QT_MOC_LITERAL(10, 200, 29), // "on_Button_meshfitting_clicked"
QT_MOC_LITERAL(11, 230, 23), // "on_Button_mesh1_clicked"
QT_MOC_LITERAL(12, 254, 23), // "on_Button_mesh2_clicked"
QT_MOC_LITERAL(13, 278, 22), // "on_Button_mult_clicked"
QT_MOC_LITERAL(14, 301, 21) // "on_Button_Occ_clicked"

    },
    "flowermodeling\0on_Button_scan_clicked\0"
    "\0on_Button_center_clicked\0"
    "on_Button_paint_clicked\0on_Button_done_clicked\0"
    "SetBrushsize\0on_Button_template_clicked\0"
    "on_Button_surface_clicked\0"
    "on_Button_mesh_clicked\0"
    "on_Button_meshfitting_clicked\0"
    "on_Button_mesh1_clicked\0on_Button_mesh2_clicked\0"
    "on_Button_mult_clicked\0on_Button_Occ_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_flowermodeling[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void flowermodeling::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        flowermodeling *_t = static_cast<flowermodeling *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_Button_scan_clicked(); break;
        case 1: _t->on_Button_center_clicked(); break;
        case 2: _t->on_Button_paint_clicked(); break;
        case 3: _t->on_Button_done_clicked(); break;
        case 4: _t->SetBrushsize(); break;
        case 5: _t->on_Button_template_clicked(); break;
        case 6: _t->on_Button_surface_clicked(); break;
        case 7: _t->on_Button_mesh_clicked(); break;
        case 8: _t->on_Button_meshfitting_clicked(); break;
        case 9: _t->on_Button_mesh1_clicked(); break;
        case 10: _t->on_Button_mesh2_clicked(); break;
        case 11: _t->on_Button_mult_clicked(); break;
        case 12: _t->on_Button_Occ_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject flowermodeling::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_flowermodeling.data,
      qt_meta_data_flowermodeling,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *flowermodeling::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *flowermodeling::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_flowermodeling.stringdata0))
        return static_cast<void*>(const_cast< flowermodeling*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int flowermodeling::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE