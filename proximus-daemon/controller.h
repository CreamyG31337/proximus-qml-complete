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
#include <QtDBus>
#include <calwrapper.h>
#include <qmheartbeat.h>
#include <QQueue>
#include <qmdevicemode.h>
#include <QSystemBatteryInfo>


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

class DataWifi : public QObject
{
    Q_OBJECT
public:
    bool enabled;
    bool inverseCond;
    bool active;
    QString SSIDs;
public Q_SLOTS:
    void activated();
    void deactivated();
Q_SIGNALS:
    void activeChanged(Rule* ruleStruct);
};

class DataDayOfWeek : public QObject
{
    Q_OBJECT
public:
    bool enabled;
    bool inverseCond;
    bool active;
    QVariant daysSelected;
public Q_SLOTS:
    void activated();
    void deactivated();
Q_SIGNALS:
    void activeChanged(Rule* ruleStruct);
};

class DataCharging : public QObject
{
    Q_OBJECT
public:
    bool enabled;
    bool inverseCond;
    bool active;
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
    DataWifi wifiRule;
    DataDayOfWeek weekdayRule;
    DataCharging chargingRule;
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
    int RuleNumber;
    bool waitingForWiFiScan;
};

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    //destructor
    virtual ~Controller();
    QPointer<QGeoPositionInfoSource> locationDataSource;
    void startExternalTimer(int min, int max);
    QSystemAlignedTimer externalTimer; //can be used by other processes not written with Qt / cpp
signals:

private Q_SLOTS:
    /**
     * Initializes one area monitor, returns pointer to it.
     */
    QGeoAreaMonitor * initAreaMonitor(DataLocation *& Dataloc);
    /**
     * Starts to monitor updates in the number of satellites.
     */

    void updateCalendar();

    void checkStatus(Rule* ruleStruct);

    void rulesStorageChanged();

    void recvScan(const QDBusMessage &msg);

    void requestScan();

    void didSomething(QString strInfo);

    void itsMidnight();
    void checkQueuedRules();
    void switchProfileNow(); //sets profile to lastRequestedProfile
    void chargingStateChanged(QSystemBatteryInfo::ChargingState state);

private:
    QPointer<QGeoSatelliteInfoSource> satelliteInfoSource;

    /**
     * Obtains the location data source and starts listening for position
     * changes.
     */
    void startGPS();
    QPointer<QSettings> settings;
    QMap<int, Rule*> Rules;
    QPointer<QFileSystemWatcher> fswatcher;
    QPointer<QSystemAlignedTimer> calTimer;
    QOrganizerManager defaultManager;
    QStringList nearbySSIDs;
    QPointer<QSystemAlignedTimer> wifiTimer;
    int pendingScan;
    QSystemAlignedTimer waitAndReScanTimer; //when a scan is aborted because the wifi is busy, wait a bit and retry the scan.    
    QPointer<CalWrapper> myCalWrapper; //I don't trust this thing to clean up when destroyed based on QOrganizerManager leaking like a sieve. Better to just hold onto one ref for now.
    QSystemAlignedTimer midnightTimer; //goes off at midnight to support the DaysOfWeek rule; started from updateCalen
    int CurrentDayOfWeek; //0-6; Sunday = 0, Monday = 1, etc
    //QQueue<Rule*> pendingRuleQueue; //filled by checkStatus() when a wifi scan is pending
    QSystemAlignedTimer profileSwitchTimer; //calls switchProfileNow() a few seconds after being started
    QString lastRequestedProfile; //updated every time a rule wants to switch profiles
    QMap<QString, bool> ruleCommandAlreadyStarted; //rulename, true set when a command is run, won't be run again until false. false is set when rule evaluates to false
    bool deviceIsCharging; //true if charging, false if discharging
    QSystemBatteryInfo myBatteryInfo;
    QSystemAlignedTimer waitForScanTimer; //when a scan result is pending we wait a few seconds for the results before evaluating the rules
};

#endif // CONTROLLER_H
