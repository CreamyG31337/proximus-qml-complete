/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created: Mon Jan 16 01:03:10 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataLocation[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   46,   13,   13, 0x0a,
      83,   46,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataLocation[] = {
    "DataLocation\0\0ruleStruct\0activeChanged(Rule*)\0"
    "update\0areaEntered(QGeoPositionInfo)\0"
    "areaExited(QGeoPositionInfo)\0"
};

const QMetaObject DataLocation::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataLocation,
      qt_meta_data_DataLocation, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataLocation::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataLocation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataLocation::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataLocation))
        return static_cast<void*>(const_cast< DataLocation*>(this));
    return QObject::qt_metacast(_clname);
}

int DataLocation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: activeChanged((*reinterpret_cast< Rule*(*)>(_a[1]))); break;
        case 1: areaEntered((*reinterpret_cast< const QGeoPositionInfo(*)>(_a[1]))); break;
        case 2: areaExited((*reinterpret_cast< const QGeoPositionInfo(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DataLocation::activeChanged(Rule * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_DataTime[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,    9,    9,    9, 0x0a,
      54,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataTime[] = {
    "DataTime\0\0ruleStruct\0activeChanged(Rule*)\0"
    "activated()\0deactivated()\0"
};

const QMetaObject DataTime::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataTime,
      qt_meta_data_DataTime, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataTime::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataTime::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataTime::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataTime))
        return static_cast<void*>(const_cast< DataTime*>(this));
    return QObject::qt_metacast(_clname);
}

int DataTime::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: activeChanged((*reinterpret_cast< Rule*(*)>(_a[1]))); break;
        case 1: activated(); break;
        case 2: deactivated(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DataTime::activeChanged(Rule * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_DataCalendar[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   13,   13,   13, 0x0a,
      58,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataCalendar[] = {
    "DataCalendar\0\0ruleStruct\0activeChanged(Rule*)\0"
    "activated()\0deactivated()\0"
};

const QMetaObject DataCalendar::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataCalendar,
      qt_meta_data_DataCalendar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataCalendar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataCalendar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataCalendar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataCalendar))
        return static_cast<void*>(const_cast< DataCalendar*>(this));
    return QObject::qt_metacast(_clname);
}

int DataCalendar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: activeChanged((*reinterpret_cast< Rule*(*)>(_a[1]))); break;
        case 1: activated(); break;
        case 2: deactivated(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DataCalendar::activeChanged(Rule * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_DataWifi[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,    9,    9,    9, 0x0a,
      54,    9,    9,    9, 0x0a,
      68,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataWifi[] = {
    "DataWifi\0\0ruleStruct\0activeChanged(Rule*)\0"
    "activated()\0deactivated()\0pretendChanged()\0"
};

const QMetaObject DataWifi::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataWifi,
      qt_meta_data_DataWifi, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataWifi::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataWifi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataWifi::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataWifi))
        return static_cast<void*>(const_cast< DataWifi*>(this));
    return QObject::qt_metacast(_clname);
}

int DataWifi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: activeChanged((*reinterpret_cast< Rule*(*)>(_a[1]))); break;
        case 1: activated(); break;
        case 2: deactivated(); break;
        case 3: pretendChanged(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DataWifi::activeChanged(Rule * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_Rule[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_Rule[] = {
    "Rule\0"
};

const QMetaObject Rule::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Rule,
      qt_meta_data_Rule, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Rule::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Rule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Rule::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Rule))
        return static_cast<void*>(const_cast< Rule*>(this));
    return QObject::qt_metacast(_clname);
}

int Rule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Controller[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      37,   29,   12,   11, 0x08,
      69,   11,   11,   11, 0x08,
      97,   86,   11,   11, 0x08,
     116,   11,   11,   11, 0x08,
     142,  138,   11,   11, 0x08,
     165,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Controller[] = {
    "Controller\0\0QGeoAreaMonitor*\0Dataloc\0"
    "initAreaMonitor(DataLocation*&)\0"
    "updateCalendar()\0ruleStruct\0"
    "checkStatus(Rule*)\0rulesStorageChanged()\0"
    "msg\0recvScan(QDBusMessage)\0requestScan()\0"
};

const QMetaObject Controller::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Controller,
      qt_meta_data_Controller, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Controller::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Controller))
        return static_cast<void*>(const_cast< Controller*>(this));
    return QObject::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QGeoAreaMonitor* _r = initAreaMonitor((*reinterpret_cast< DataLocation*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QGeoAreaMonitor**>(_a[0]) = _r; }  break;
        case 1: updateCalendar(); break;
        case 2: checkStatus((*reinterpret_cast< Rule*(*)>(_a[1]))); break;
        case 3: rulesStorageChanged(); break;
        case 4: recvScan((*reinterpret_cast< const QDBusMessage(*)>(_a[1]))); break;
        case 5: requestScan(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
