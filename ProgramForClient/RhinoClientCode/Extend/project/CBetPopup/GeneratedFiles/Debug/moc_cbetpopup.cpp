/****************************************************************************
** Meta object code from reading C++ file 'cbetpopup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cbetpopup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cbetpopup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CBetPopup_t {
    QByteArrayData data[9];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBetPopup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBetPopup_t qt_meta_stringdata_CBetPopup = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CBetPopup"
QT_MOC_LITERAL(1, 10, 17), // "onLoadUrlFinished"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "_status"
QT_MOC_LITERAL(4, 37, 17), // "onAnimateFinished"
QT_MOC_LITERAL(5, 55, 11), // "getdlgMatch"
QT_MOC_LITERAL(6, 67, 8), // "QWidget*"
QT_MOC_LITERAL(7, 76, 21), // "setWindMousePenetrate"
QT_MOC_LITERAL(8, 98, 10) // "bPenetrate"

    },
    "CBetPopup\0onLoadUrlFinished\0\0_status\0"
    "onAnimateFinished\0getdlgMatch\0QWidget*\0"
    "setWindMousePenetrate\0bPenetrate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBetPopup[] = {

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

void CBetPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CBetPopup *_t = static_cast<CBetPopup *>(_o);
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

const QMetaObject CBetPopup::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CBetPopup.data,
      qt_meta_data_CBetPopup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBetPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBetPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBetPopup.stringdata0))
        return static_cast<void*>(const_cast< CBetPopup*>(this));
    return QDialog::qt_metacast(_clname);
}

int CBetPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
