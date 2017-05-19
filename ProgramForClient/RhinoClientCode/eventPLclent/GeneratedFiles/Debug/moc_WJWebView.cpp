/****************************************************************************
** Meta object code from reading C++ file 'WJWebView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../web/WJWebView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WJWebView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WJWebView_t {
    QByteArrayData data[6];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WJWebView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WJWebView_t qt_meta_stringdata_WJWebView = {
    {
QT_MOC_LITERAL(0, 0, 9), // "WJWebView"
QT_MOC_LITERAL(1, 10, 13), // "onLoadStarted"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "onLoadProgress"
QT_MOC_LITERAL(4, 40, 8), // "progress"
QT_MOC_LITERAL(5, 49, 14) // "onLoadFinished"

    },
    "WJWebView\0onLoadStarted\0\0onLoadProgress\0"
    "progress\0onLoadFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WJWebView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       5,    1,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void WJWebView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WJWebView *_t = static_cast<WJWebView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onLoadStarted(); break;
        case 1: _t->onLoadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject WJWebView::staticMetaObject = {
    { &QWebEngineView::staticMetaObject, qt_meta_stringdata_WJWebView.data,
      qt_meta_data_WJWebView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WJWebView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WJWebView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WJWebView.stringdata0))
        return static_cast<void*>(const_cast< WJWebView*>(this));
    return QWebEngineView::qt_metacast(_clname);
}

int WJWebView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebEngineView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
