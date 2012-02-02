/****************************************************************************
** Meta object code from reading C++ file 'dbusiface.h'
**
** Created: Wed Feb 1 23:59:01 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dbusiface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dbusiface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DbusIface[] = {

 // content:
       5,       // revision
       0,       // classname
       1,   14, // classinfo
       3,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
      32,   10,

 // signals: signature, parameters, type, tag, flags
      49,   48,   48,   48, 0x05,
      69,   61,   48,   48, 0x05,

 // slots: signature, parameters, type, tag, flags
     104,   96,   91,   48, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DbusIface[] = {
    "DbusIface\0net.appcheck.Proximus\0"
    "D-Bus Interface\0\0heartbeat()\0strInfo\0"
    "didSomething(QString)\0bool\0min,max\0"
    "startTimer(int,int)\0"
};

const QMetaObject DbusIface::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_DbusIface,
      qt_meta_data_DbusIface, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DbusIface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DbusIface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DbusIface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DbusIface))
        return static_cast<void*>(const_cast< DbusIface*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int DbusIface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: heartbeat(); break;
        case 1: didSomething((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: { bool _r = startTimer((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DbusIface::heartbeat()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DbusIface::didSomething(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
