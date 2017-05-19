/****************************************************************************
** Meta object code from reading C++ file 'WJMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../web/WJMainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WJMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CWJMainWindow_t {
    QByteArrayData data[19];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CWJMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CWJMainWindow_t qt_meta_stringdata_CWJMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CWJMainWindow"
QT_MOC_LITERAL(1, 14, 14), // "signa_linkland"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 10), // "mouseEvent"
QT_MOC_LITERAL(4, 41, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(5, 75, 6), // "reason"
QT_MOC_LITERAL(6, 82, 5), // "hide1"
QT_MOC_LITERAL(7, 88, 18), // "ExitMonitorProcess"
QT_MOC_LITERAL(8, 107, 8), // "setEvent"
QT_MOC_LITERAL(9, 116, 9), // "taskEvent"
QT_MOC_LITERAL(10, 126, 13), // "MsgBoxGeneral"
QT_MOC_LITERAL(11, 140, 5), // "title"
QT_MOC_LITERAL(12, 146, 6), // "bClose"
QT_MOC_LITERAL(13, 153, 12), // "MsgBoxSignal"
QT_MOC_LITERAL(14, 166, 5), // "okBtn"
QT_MOC_LITERAL(15, 172, 5), // "noBtn"
QT_MOC_LITERAL(16, 178, 15), // "DoMsgBoxGeneral"
QT_MOC_LITERAL(17, 194, 14), // "DoMsgBoxSignal"
QT_MOC_LITERAL(18, 209, 21) // "onTime_update_players"

    },
    "CWJMainWindow\0signa_linkland\0\0mouseEvent\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0hide1\0ExitMonitorProcess\0"
    "setEvent\0taskEvent\0MsgBoxGeneral\0title\0"
    "bClose\0MsgBoxSignal\0okBtn\0noBtn\0"
    "DoMsgBoxGeneral\0DoMsgBoxSignal\0"
    "onTime_update_players"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CWJMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   77,    2, 0x0a /* Public */,
       6,    0,   80,    2, 0x0a /* Public */,
       7,    0,   81,    2, 0x0a /* Public */,
       8,    0,   82,    2, 0x0a /* Public */,
       9,    0,   83,    2, 0x0a /* Public */,
      10,    2,   84,    2, 0x0a /* Public */,
      10,    1,   89,    2, 0x2a /* Public | MethodCloned */,
      13,    3,   92,    2, 0x0a /* Public */,
      16,    0,   99,    2, 0x0a /* Public */,
      17,    0,  100,    2, 0x0a /* Public */,
      18,    0,  101,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   11,   12,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   11,   14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CWJMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CWJMainWindow *_t = static_cast<CWJMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signa_linkland((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->mouseEvent((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 2: _t->hide1(); break;
        case 3: _t->ExitMonitorProcess(); break;
        case 4: _t->setEvent(); break;
        case 5: _t->taskEvent(); break;
        case 6: _t->MsgBoxGeneral((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: _t->MsgBoxGeneral((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->MsgBoxSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 9: _t->DoMsgBoxGeneral(); break;
        case 10: _t->DoMsgBoxSignal(); break;
        case 11: _t->onTime_update_players(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CWJMainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CWJMainWindow::signa_linkland)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CWJMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CWJMainWindow.data,
      qt_meta_data_CWJMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CWJMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CWJMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CWJMainWindow.stringdata0))
        return static_cast<void*>(const_cast< CWJMainWindow*>(this));
    if (!strcmp(_clname, "ZSQtMouseDrag"))
        return static_cast< ZSQtMouseDrag*>(const_cast< CWJMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CWJMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void CWJMainWindow::signa_linkland(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
