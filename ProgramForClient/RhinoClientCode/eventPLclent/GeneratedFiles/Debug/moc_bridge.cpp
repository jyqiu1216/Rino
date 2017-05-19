/****************************************************************************
** Meta object code from reading C++ file 'bridge.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../bridge.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bridge.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_bridge_t {
    QByteArrayData data[17];
    char stringdata0[217];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_bridge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_bridge_t qt_meta_stringdata_bridge = {
    {
QT_MOC_LITERAL(0, 0, 6), // "bridge"
QT_MOC_LITERAL(1, 7, 15), // "slgnal_webclose"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 18), // "slgnal_webpagclose"
QT_MOC_LITERAL(4, 43, 18), // "slgnal_webejectpag"
QT_MOC_LITERAL(5, 62, 15), // "slgnal_webeject"
QT_MOC_LITERAL(6, 78, 17), // "slgnal_setwebsize"
QT_MOC_LITERAL(7, 96, 20), // "slgnal_setwebpagsize"
QT_MOC_LITERAL(8, 117, 17), // "signal_sendResult"
QT_MOC_LITERAL(9, 135, 4), // "text"
QT_MOC_LITERAL(10, 140, 8), // "webclose"
QT_MOC_LITERAL(11, 149, 11), // "webpagclose"
QT_MOC_LITERAL(12, 161, 11), // "webejectpag"
QT_MOC_LITERAL(13, 173, 8), // "webeject"
QT_MOC_LITERAL(14, 182, 10), // "setwebsize"
QT_MOC_LITERAL(15, 193, 13), // "setwebpagsize"
QT_MOC_LITERAL(16, 207, 9) // "retcclose"

    },
    "bridge\0slgnal_webclose\0\0slgnal_webpagclose\0"
    "slgnal_webejectpag\0slgnal_webeject\0"
    "slgnal_setwebsize\0slgnal_setwebpagsize\0"
    "signal_sendResult\0text\0webclose\0"
    "webpagclose\0webejectpag\0webeject\0"
    "setwebsize\0setwebpagsize\0retcclose"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_bridge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,
       4,    1,   86,    2, 0x06 /* Public */,
       5,    1,   89,    2, 0x06 /* Public */,
       6,    2,   92,    2, 0x06 /* Public */,
       7,    2,   97,    2, 0x06 /* Public */,
       8,    1,  102,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,  105,    2, 0x0a /* Public */,
      11,    0,  106,    2, 0x0a /* Public */,
      12,    1,  107,    2, 0x0a /* Public */,
      13,    1,  110,    2, 0x0a /* Public */,
      14,    2,  113,    2, 0x0a /* Public */,
      15,    2,  118,    2, 0x0a /* Public */,
      16,    0,  123,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,

       0        // eod
};

void bridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        bridge *_t = static_cast<bridge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slgnal_webclose(); break;
        case 1: _t->slgnal_webpagclose(); break;
        case 2: _t->slgnal_webejectpag((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->slgnal_webeject((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->slgnal_setwebsize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->slgnal_setwebpagsize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->signal_sendResult((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->webclose(); break;
        case 8: _t->webpagclose(); break;
        case 9: _t->webejectpag((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->webeject((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->setwebsize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->setwebpagsize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->retcclose(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (bridge::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bridge::slgnal_webclose)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (bridge::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bridge::slgnal_webpagclose)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (bridge::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bridge::slgnal_webejectpag)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (bridge::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bridge::slgnal_webeject)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (bridge::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bridge::slgnal_setwebsize)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (bridge::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bridge::slgnal_setwebpagsize)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (bridge::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bridge::signal_sendResult)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject bridge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_bridge.data,
      qt_meta_data_bridge,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *bridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *bridge::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_bridge.stringdata0))
        return static_cast<void*>(const_cast< bridge*>(this));
    return QObject::qt_metacast(_clname);
}

int bridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void bridge::slgnal_webclose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void bridge::slgnal_webpagclose()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void bridge::slgnal_webejectpag(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void bridge::slgnal_webeject(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void bridge::slgnal_setwebsize(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void bridge::slgnal_setwebpagsize(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void bridge::signal_sendResult(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
