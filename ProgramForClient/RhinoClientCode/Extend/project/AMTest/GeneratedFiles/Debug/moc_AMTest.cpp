/****************************************************************************
** Meta object code from reading C++ file 'AMTest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AMTest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AMTest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AMTest_t {
    QByteArrayData data[20];
    char stringdata0[343];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AMTest_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AMTest_t qt_meta_stringdata_AMTest = {
    {
QT_MOC_LITERAL(0, 0, 6), // "AMTest"
QT_MOC_LITERAL(1, 7, 11), // "onActivated"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 9), // "onExecute"
QT_MOC_LITERAL(4, 30, 12), // "onClick_test"
QT_MOC_LITERAL(5, 43, 17), // "onClick_reloadLua"
QT_MOC_LITERAL(6, 61, 19), // "onClick_lol_startup"
QT_MOC_LITERAL(7, 81, 18), // "onClick_lol_create"
QT_MOC_LITERAL(8, 100, 16), // "onClick_lol_join"
QT_MOC_LITERAL(9, 117, 16), // "onClick_lol_play"
QT_MOC_LITERAL(10, 134, 20), // "onClick_hs_injectDll"
QT_MOC_LITERAL(11, 155, 20), // "onClick_hs_reqFrient"
QT_MOC_LITERAL(12, 176, 23), // "onClick_hs_reqChallenge"
QT_MOC_LITERAL(13, 200, 26), // "onClick_hs_acceptChallenge"
QT_MOC_LITERAL(14, 227, 23), // "onClick_hs_acceptFriend"
QT_MOC_LITERAL(15, 251, 18), // "onClick_hs_msg2005"
QT_MOC_LITERAL(16, 270, 18), // "onClick_hs_msg2006"
QT_MOC_LITERAL(17, 289, 16), // "onClick_hs_pipei"
QT_MOC_LITERAL(18, 306, 18), // "onClick_hs_endGame"
QT_MOC_LITERAL(19, 325, 17) // "onEmbedWinHSClose"

    },
    "AMTest\0onActivated\0\0onExecute\0"
    "onClick_test\0onClick_reloadLua\0"
    "onClick_lol_startup\0onClick_lol_create\0"
    "onClick_lol_join\0onClick_lol_play\0"
    "onClick_hs_injectDll\0onClick_hs_reqFrient\0"
    "onClick_hs_reqChallenge\0"
    "onClick_hs_acceptChallenge\0"
    "onClick_hs_acceptFriend\0onClick_hs_msg2005\0"
    "onClick_hs_msg2006\0onClick_hs_pipei\0"
    "onClick_hs_endGame\0onEmbedWinHSClose"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AMTest[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x0a /* Public */,
       3,    0,  107,    2, 0x0a /* Public */,
       4,    0,  108,    2, 0x0a /* Public */,
       5,    0,  109,    2, 0x0a /* Public */,
       6,    0,  110,    2, 0x0a /* Public */,
       7,    0,  111,    2, 0x0a /* Public */,
       8,    0,  112,    2, 0x0a /* Public */,
       9,    0,  113,    2, 0x0a /* Public */,
      10,    0,  114,    2, 0x0a /* Public */,
      11,    0,  115,    2, 0x0a /* Public */,
      12,    0,  116,    2, 0x0a /* Public */,
      13,    0,  117,    2, 0x0a /* Public */,
      14,    0,  118,    2, 0x0a /* Public */,
      15,    0,  119,    2, 0x0a /* Public */,
      16,    0,  120,    2, 0x0a /* Public */,
      17,    0,  121,    2, 0x0a /* Public */,
      18,    0,  122,    2, 0x0a /* Public */,
      19,    0,  123,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AMTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AMTest *_t = static_cast<AMTest *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onActivated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onExecute(); break;
        case 2: _t->onClick_test(); break;
        case 3: _t->onClick_reloadLua(); break;
        case 4: _t->onClick_lol_startup(); break;
        case 5: _t->onClick_lol_create(); break;
        case 6: _t->onClick_lol_join(); break;
        case 7: _t->onClick_lol_play(); break;
        case 8: _t->onClick_hs_injectDll(); break;
        case 9: _t->onClick_hs_reqFrient(); break;
        case 10: _t->onClick_hs_reqChallenge(); break;
        case 11: _t->onClick_hs_acceptChallenge(); break;
        case 12: _t->onClick_hs_acceptFriend(); break;
        case 13: _t->onClick_hs_msg2005(); break;
        case 14: _t->onClick_hs_msg2006(); break;
        case 15: _t->onClick_hs_pipei(); break;
        case 16: _t->onClick_hs_endGame(); break;
        case 17: _t->onEmbedWinHSClose(); break;
        default: ;
        }
    }
}

const QMetaObject AMTest::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AMTest.data,
      qt_meta_data_AMTest,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AMTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AMTest::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AMTest.stringdata0))
        return static_cast<void*>(const_cast< AMTest*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int AMTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
