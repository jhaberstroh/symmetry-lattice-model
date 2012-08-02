/****************************************************************************
** Meta object code from reading C++ file 'functionthread.h'
**
** Created: Thu Aug 2 12:20:37 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "functionthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'functionthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FunctionThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   42,   15,   15, 0x0a,
      92,   70,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FunctionThread[] = {
    "FunctionThread\0\0output\0sendOutput(double)\0"
    "checked\0on_go_toggled(bool)\0"
    "new_parameter_value,p\0"
    "on_parameter_changed(double,Parameter)\0"
};

void FunctionThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FunctionThread *_t = static_cast<FunctionThread *>(_o);
        switch (_id) {
        case 0: _t->sendOutput((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->on_go_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_parameter_changed((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< Parameter(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FunctionThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FunctionThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_FunctionThread,
      qt_meta_data_FunctionThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FunctionThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FunctionThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FunctionThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FunctionThread))
        return static_cast<void*>(const_cast< FunctionThread*>(this));
    return QThread::qt_metacast(_clname);
}

int FunctionThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FunctionThread::sendOutput(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
