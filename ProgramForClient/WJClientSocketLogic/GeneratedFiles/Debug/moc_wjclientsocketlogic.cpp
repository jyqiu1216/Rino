/****************************************************************************
** Meta object code from reading C++ file 'wjclientsocketlogic.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../sdk/clientsocket/wjclientsocketlogic.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wjclientsocketlogic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WJ__WJClientSocketLogic_t {
    QByteArrayData data[3];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WJ__WJClientSocketLogic_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WJ__WJClientSocketLogic_t qt_meta_stringdata_WJ__WJClientSocketLogic = {
    {
QT_MOC_LITERAL(0, 0, 23), // "WJ::WJClientSocketLogic"
QT_MOC_LITERAL(1, 24, 18), // "socketDataDispatch"
QT_MOC_LITERAL(2, 43, 0) // ""

    },
    "WJ::WJClientSocketLogic\0socketDataDispatch\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WJ__WJClientSocketLogic[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void WJ::WJClientSocketLogic::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WJClientSocketLogic *_t = static_cast<WJClientSocketLogic *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->socketDataDispatch(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject WJ::WJClientSocketLogic::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WJ__WJClientSocketLogic.data,
      qt_meta_data_WJ__WJClientSocketLogic,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WJ::WJClientSocketLogic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WJ::WJClientSocketLogic::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WJ__WJClientSocketLogic.stringdata0))
        return static_cast<void*>(const_cast< WJClientSocketLogic*>(this));
    if (!strcmp(_clname, "ISocketEventDelegate"))
        return static_cast< ISocketEventDelegate*>(const_cast< WJClientSocketLogic*>(this));
    return QObject::qt_metacast(_clname);
}

int WJ::WJClientSocketLogic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
