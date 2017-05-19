/****************************************************************************
** Meta object code from reading C++ file 'betwindowlol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../betwindowlol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'betwindowlol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BetWindowLoL_t {
    QByteArrayData data[9];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BetWindowLoL_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BetWindowLoL_t qt_meta_stringdata_BetWindowLoL = {
    {
QT_MOC_LITERAL(0, 0, 12), // "BetWindowLoL"
QT_MOC_LITERAL(1, 13, 17), // "onLoadUrlFinished"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 7), // "_status"
QT_MOC_LITERAL(4, 40, 17), // "onAnimateFinished"
QT_MOC_LITERAL(5, 58, 11), // "getdlgMatch"
QT_MOC_LITERAL(6, 70, 8), // "QWidget*"
QT_MOC_LITERAL(7, 79, 21), // "setWindMousePenetrate"
QT_MOC_LITERAL(8, 101, 10) // "bPenetrate"

    },
    "BetWindowLoL\0onLoadUrlFinished\0\0_status\0"
    "onAnimateFinished\0getdlgMatch\0QWidget*\0"
    "setWindMousePenetrate\0bPenetrate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BetWindowLoL[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    0,   37,    2, 0x0a /* Public */,
       5,    0,   38,    2, 0x0a /* Public */,
       7,    1,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    0x80000000 | 6,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void BetWindowLoL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BetWindowLoL *_t = static_cast<BetWindowLoL *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onLoadUrlFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onAnimateFinished(); break;
        case 2: { QWidget* _r = _t->getdlgMatch();
            if (_a[0]) *reinterpret_cast< QWidget**>(_a[0]) = _r; }  break;
        case 3: _t->setWindMousePenetrate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject BetWindowLoL::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BetWindowLoL.data,
      qt_meta_data_BetWindowLoL,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BetWindowLoL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BetWindowLoL::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BetWindowLoL.stringdata0))
        return static_cast<void*>(const_cast< BetWindowLoL*>(this));
    return QDialog::qt_metacast(_clname);
}

int BetWindowLoL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
