/****************************************************************************
** Meta object code from reading C++ file 'dbusiface.h'
**
** Created: Wed Feb 1 23:58:48 2012
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
       2,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      35,   10,

 // signals: signature, parameters, type, tag, flags
      60,   52,   51,   51, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   81,   51,   51, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DbusIface[] = {
    "DbusIface\0net.appcheck.Proximus.UI\0"
    "D-Bus Interface\0\0logText\0newLogInfo(QVariant)\0"
    "text\0updateLog(QString)\0"
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
        case 0: newLogInfo((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 1: updateLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DbusIface::newLogInfo(const QVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
