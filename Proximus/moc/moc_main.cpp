/****************************************************************************
** Meta object code from reading C++ file 'main.h'
**
** Created: Wed Dec 28 03:10:01 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../main.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProximusUtils[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      23,   14,   15,   14, 0x02,
      42,   14,   14,   14, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_ProximusUtils[] = {
    "ProximusUtils\0\0QString\0isServiceRunning()\0"
    "refreshRulesModel()\0"
};

const QMetaObject ProximusUtils::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ProximusUtils,
      qt_meta_data_ProximusUtils, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProximusUtils::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProximusUtils::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProximusUtils::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProximusUtils))
        return static_cast<void*>(const_cast< ProximusUtils*>(this));
    return QObject::qt_metacast(_clname);
}

int ProximusUtils::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = isServiceRunning();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: refreshRulesModel(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_MySettings[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      39,   21,   12,   11, 0x02,
      76,   66,   11,   11, 0x02,
     110,  103,   11,   11, 0x02,
     130,   11,   11,   11, 0x02,
     150,  146,  141,   11, 0x02,
     180,   11,  168,   11, 0x02,
     202,   11,  194,   11, 0x02,
     210,  146,   11,   11, 0x02,
     226,   11,  194,   11, 0x02,
     237,   11,   11,   11, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_MySettings[] = {
    "MySettings\0\0QVariant\0keyval,defaultval\0"
    "getValue(QString,QVariant)\0key,value\0"
    "setValue(QString,QVariant)\0prefix\0"
    "beginGroup(QString)\0endGroup()\0bool\0"
    "key\0contains(QString)\0QStringList\0"
    "childGroups()\0QString\0group()\0"
    "remove(QString)\0fileName()\0clear()\0"
};

const QMetaObject MySettings::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MySettings,
      qt_meta_data_MySettings, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MySettings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MySettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MySettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MySettings))
        return static_cast<void*>(const_cast< MySettings*>(this));
    return QObject::qt_metacast(_clname);
}

int MySettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QVariant _r = getValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 1: setValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 2: beginGroup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: endGroup(); break;
        case 4: { bool _r = contains((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { QStringList _r = childGroups();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 6: { QString _r = group();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: remove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: { QString _r = fileName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 9: clear(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
static const uint qt_meta_data_RuleObject[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       2,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // properties: name, type, flags
      34,   29, 0x01495103,
      50,   42, 0x0a495103,

 // properties: notify_signal_id
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_RuleObject[] = {
    "RuleObject\0\0myModelChanged()\0bool\0"
    "enabled\0QString\0name\0"
};

const QMetaObject RuleObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RuleObject,
      qt_meta_data_RuleObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RuleObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RuleObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RuleObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RuleObject))
        return static_cast<void*>(const_cast< RuleObject*>(this));
    return QObject::qt_metacast(_clname);
}

int RuleObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: myModelChanged(); break;
        default: ;
        }
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = enabled(); break;
        case 1: *reinterpret_cast< QString*>(_v) = name(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 1: setName(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void RuleObject::myModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
