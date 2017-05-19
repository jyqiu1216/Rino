/****************************************************************************
** Meta object code from reading C++ file 'EmbedWindowHS.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../eventPLclent/hook/EmbedWindowHS.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EmbedWindowHS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_EmbedWindowHS_t {
    QByteArrayData data[10];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EmbedWindowHS_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EmbedWindowHS_t qt_meta_stringdata_EmbedWindowHS = {
    {
QT_MOC_LITERAL(0, 0, 13), // "EmbedWindowHS"
QT_MOC_LITERAL(1, 14, 17), // "onLoadUrlFinished"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 7), // "_status"
QT_MOC_LITERAL(4, 41, 17), // "onAnimateFinished"
QT_MOC_LITERAL(5, 59, 11), // "getdlgMatch"
QT_MOC_LITERAL(6, 71, 8), // "QWidget*"
QT_MOC_LITERAL(7, 80, 13), // "getdlgLoading"
QT_MOC_LITERAL(8, 94, 21), // "setWindMousePenetrate"
QT_MOC_LITERAL(9, 116, 10) // "bPenetrate"

    },
    "EmbedWindowHS\0onLoadUrlFinished\0\0"
    "_status\0onAnimateFinished\0getdlgMatch\0"
    "QWidget*\0getdlgLoading\0setWindMousePenetrate\0"
    "bPenetrate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EmbedWindowHS[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    0,   42,    2, 0x0a /* Public */,
       5,    0,   43,    2, 0x0a /* Public */,
       7,    0,   44,    2, 0x0a /* Public */,
       8,    1,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    0x80000000 | 6,
    0x80000000 | 6,
    QMetaType::Void, QMetaType::Bool,    9,

       0        // eod
};

void EmbedWindowHS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EmbedWindowHS *_t = static_cast<EmbedWindowHS *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onLoadUrlFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onAnimateFinished(); break;
        case 2: { QWidget* _r = _t->getdlgMatch();
            if (_a[0]) *reinterpret_cast< QWidget**>(_a[0]) = _r; }  break;
        case 3: { QWidget* _r = _t->getdlgLoading();
            if (_a[0]) *reinterpret_cast< QWidget**>(_a[0]) = _r; }  break;
        case 4: _t->setWindMousePenetrate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject EmbedWindowHS::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EmbedWindowHS.data,
      qt_meta_data_EmbedWindowHS,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EmbedWindowHS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EmbedWindowHS::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EmbedWindowHS.stringdata0))
        return static_cast<void*>(const_cast< EmbedWindowHS*>(this));
    return QDialog::qt_metacast(_clname);
}

int EmbedWindowHS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
