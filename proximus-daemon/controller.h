#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtCore/QCoreApplication>
#include <QtGui/QMainWindow>
#include <QtSystemInfo/QSystemDeviceInfo>
#include <QtCore/QPointer>
#include <QtLocation/QGeoPositionInfoSource>
#include <QtLocation/QGeoPositionInfo>
#include <QtLocation/QGeoSatelliteInfoSource>
#include <QtLocation/QGeoSatelliteInfo>
#include <QtLocation/QGeoCoordinate>
#include <QtLocation/QGeoAreaMonitor>
#include <QtGui/QMessageBox>
#include <QtCore/QTime>
#include <QtOrganizer/QOrganizerManager>
#include <QtOrganizer/QOrganizerItem>
#include <QtCore/QTimer>
#include <QtOrganizer/QOrganizerEventTime>
#include <QtOrganizer/QOrganizerItemDetail>
#include <QtGui/QApplication>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtCore/QFileSystemWatcher>



#if defined(Q_WS_MAEMO_5)
    //dunno
#else
    #if defined (Q_WS_SIMULATOR)
        //i dunno what to do in this case.
    #else //harmattan or symbian...
        #include <QSystemAlignedTimer>
    #endif
#endif


QTM_USE_NAMESPACE

class Rule;
class DataLocation : public QObject
{
    Q_OBJECT
public:
    explicit DataLocation();//constructor
    virtual ~DataLocation();
    bool enabled;
    bool inverseCond;
    bool active;//active means the conditions are all true
    QGeoCoordinate location;
    qint16 radius;
    QPointer<QGeoAreaMonitor> areaMon;
public Q_SLOTS:
    /**
     * Called when the current position is in range of the area.
     */
    void areaEntered(const QGeoPositionInfo &update);
    /**
     * Called when the current position moves out of range of the area.
     */
    void areaExited(const QGeoPositionInfo &update);
Q_SIGNALS:
    void activeChanged(Rule* ruleStruct);
};
class DataTime : public QObject
{
    Q_OBJECT
public:
    bool enabled;
    bool inverseCond;
    bool active;
    QTime time1;
    QTime time2;
    QTimer activateTimer;
    QTimer deactivateTimer;
public Q_SLOTS:
    void activated();
    void deactivated();
Q_SIGNALS:
    void activeChanged(Rule* ruleStruct);
};

class DataCalendar : public QObject
{
    Q_OBJECT
public:
    bool enabled;
    bool inverseCond;
    bool active;
    QString keywords;
    QTimer activateTimer;
    QTimer deactivateTimer;
public Q_SLOTS:
    void activated();
    void deactivated();
Q_SIGNALS:
    void activeChanged(Rule* ruleStruct);
};
struct RuleData
{
    explicit RuleData();
    virtual ~RuleData();
    QPointer<DataLocation> locationRule;
    DataTime timeRule;
    DataCalendar calendarRule;
};
class Rule : public QObject
{
    Q_OBJECT
public:
    explicit Rule();
    virtual ~Rule();
    QString name;
    bool enabled;
    bool active;
    RuleData data;
};

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    //destructor
    virtual ~Controller();
    QPointer<QGeoPositionInfoSource> locationDataSource;

signals:

public Q_SLOTS:
    /**
     * Called when the current position is updated.
     */
//    void positionUpdated(QGeoPositionInfo geoPositionInfo);
    /**
     * Called when the number of satellites in use is updated.
     */
//    void satellitesInUseUpdated(
//            const QList<QGeoSatelliteInfo> &satellites);
//    /**
//     * Called when the number of satellites in view is updated.
//     */
//    void satellitesInViewUpdated(
//            const QList<QGeoSatelliteInfo> &satellites);

private Q_SLOTS:
    /**
     * Initializes one area monitor, returns pointer to it.
     */
    QGeoAreaMonitor * initAreaMonitor(DataLocation *& Dataloc);
    /**
     * Starts to monitor updates in the number of satellites.
     */
//    void startSatelliteMonitor();

    void updateCalendar();

    void checkStatus(Rule* ruleStruct);

    void rulesStorageChanged();

private:
    QPointer<QGeoSatelliteInfoSource> satelliteInfoSource;

    /**
     * Obtains the location data source and starts listening for position
     * changes.
     */
    void startGPS();
    QPointer<QSettings> settings;
    QHash<QString, Rule*> Rules;
    QPointer<QFileSystemWatcher> fswatcher;
    QPointer<QSystemAlignedTimer> calTimer;

    QOrganizerManager defaultManager;
};

#endif // CONTROLLER_H
