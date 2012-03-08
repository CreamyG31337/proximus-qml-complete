#include "controller.h"
#include "profileclient.h"
#include "wlancond-dbus.h"
#include "wlancond.h"

Controller::Controller(QObject *parent) : QObject(parent)
    ,settings(new QSettings("/home/user/.config/FakeCompany/Proximus.conf",QSettings::NativeFormat,this))
    ,fswatcher(new QFileSystemWatcher(this))
    ,calTimer(new QSystemAlignedTimer(this))
    ,wifiTimer(new QSystemAlignedTimer(this))
    //,externalTimer(new QSystemAlignedTimer(this))
    ,myCalWrapper(new CalWrapper(this))
    //,myBatteryInfo(new QSystemBatteryInfo(this))

{//important to init qsettings like that so it doesn't store in /home/root/ or whatever other account name
    qDebug() << "starting proximus";
    didSomething("Proximus Daemon starting up");
    pendingScan = false;
    fswatcher->addPath("/home/user/.config/FakeCompany/Proximus.conf");
    connect(fswatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(rulesStorageChanged()));//don't need the filename passed
    QCoreApplication::setOrganizationName("FakeCompany");
    QCoreApplication::setOrganizationDomain("appcheck.net");
    QCoreApplication::setApplicationName("Proximus");

    //set correct day of week
    CurrentDayOfWeek = QDate::currentDate().dayOfWeek(); //1-7; monday = 1, sunday = 7
    if (CurrentDayOfWeek == 7) CurrentDayOfWeek = 0;


    //set initial state

    //sometimes the underlying signals don't work, not sure how often this occurs so ignored for now
    if (myBatteryInfo.chargingState() == QSystemBatteryInfo::Charging){
        deviceIsCharging = true;
        qDebug() << "apparently charging" << myBatteryInfo.chargingState();
    }
    else{
        deviceIsCharging = false;
        qDebug() << "apparently NOT charging" << myBatteryInfo.chargingState();
    }

    connect(&myBatteryInfo,SIGNAL(chargingStateChanged(QSystemBatteryInfo::ChargingState)),
                                  this, SLOT(chargingStateChanged(QSystemBatteryInfo::ChargingState)));


    connect(&profileSwitchTimer,SIGNAL(timeout()),this,SLOT(switchProfileNow()));

    connect(&this->waitForScanTimer, SIGNAL(timeout()),
            this, SLOT(checkQueuedRules()));


    //call once now to populate initial rules
    rulesStorageChanged();

    //set up timer for calendar,
    #if defined(Q_WS_MAEMO_5)
        //worry about this later
    #else
        #if defined (Q_WS_SIMULATOR)
            //
        #else //harmattan or symbian...
           // QSystemAlignedTimer *newTimer = new QSystemAlignedTimer(this);
            connect (calTimer, SIGNAL(timeout()),this,SLOT(updateCalendar()));
            calTimer->start(60*45,60*60); //timer should fire every 45-60 min
            connect (wifiTimer, SIGNAL(timeout()),this,SLOT(requestScan()));//don't start yet
            connect (&midnightTimer, SIGNAL(timeout()),this,SLOT(itsMidnight()));//don't start yet, started via updatecalendar
        #endif
    #endif

    QDBusConnection::systemBus().connect(WLANCOND_SERVICE, WLANCOND_SIG_PATH,
                                         WLANCOND_SIG_INTERFACE, WLANCOND_SCAN_RESULTS_SIG,
                                         this, SLOT(recvScan(const QDBusMessage&)));
    connect(&waitAndReScanTimer, SIGNAL(timeout()),
            this, SLOT(requestScan()));

    qDebug() << "init complete; curr dayOfWeek: " << CurrentDayOfWeek;
    didSomething("Proximus Daemon startup complete.");    
}

void Controller::startExternalTimer(int min, int max){
    externalTimer.start(min,max);
    didSomething("External Timer started - min/max: " + QString::number(min) + "/" +  QString::number(max));
}

void Controller::recvScan(const QDBusMessage &msg)
{//i'd never have figured this out without http://b.zekjur.net/post/27
    nearbySSIDs.clear();
    QList<QVariant> args = msg.arguments();
    int networks = args.value(0).value<int>();
    if (( networks > 64)|| (networks < 0)){
        didSomething("wifi dbus error - pls report this bug");
        return;
    }

    for (int c = 0; c < networks; c++) {
        int pos = 1 + (c * 5);
        nearbySSIDs.append(args.value(pos).value<QByteArray>());
    }
    qDebug() << nearbySSIDs;
    didSomething("wifi scan sees SSIDs: " + nearbySSIDs.join(", "));
    //check all rules now
    bool match;
    pendingScan = false;
    foreach(Rule* ruleStruct,  Rules) {
        match = false;
        if(ruleStruct->enabled && ruleStruct->data.wifiRule.enabled)
        {
            QStringList RuleSSIDs = ruleStruct->data.wifiRule.SSIDs.split(",");
            foreach(QString SSIDfromScan, nearbySSIDs)
            {
                foreach(QString ruleSSID,RuleSSIDs)
                {
                    if(SSIDfromScan.toUpper().contains(ruleSSID.toUpper().trimmed()))
                    {
                        match = true;
                        break;
                    }
                }
                if (match)
                    break;
            }
        }
        if(match)
            ruleStruct->data.wifiRule.activated();
        else
            ruleStruct->data.wifiRule.deactivated();
    }
}

void Controller::requestScan()
{    
    if(pendingScan){
        qDebug() << "skipped extra wifi scan request";
        return;
    }
    didSomething("requesting wifi scan");
    pendingScan = true;
    QDBusInterface remoteApp(WLANCOND_SERVICE, WLANCOND_REQ_PATH,
                              WLANCOND_REQ_INTERFACE , QDBusConnection::systemBus());

    QDBusMessage reply = remoteApp.call(WLANCOND_SCAN_REQ, WLANCOND_TX_POWER10, QByteArray (""), 0);
    if (reply.type() == QDBusMessage::ErrorMessage){
        QString errMessage = reply.errorMessage();
        if  (errMessage == "com.nokia.wlancond.error.already_active"){
            //wait and rescan
            qDebug() << "wifi already active - waiting for rescan";
            waitAndReScanTimer.stop();
            waitAndReScanTimer.start(7,13);
            waitAndReScanTimer.setSingleShot(true);
        }
        else if (errMessage == "com.nokia.wlancond.error.wlan_disabled"){
            didSomething("wifi disabled - can't scan");
            qDebug() << errMessage; //likes to get "com.nokia.wlancond.error.already_active" , and then not do a scan...
        }
        else
        {
            didSomething("wifi error - unknown - pls report: " +  errMessage);
            qDebug() << errMessage; //likes to get "com.nokia.wlancond.error.already_active" , and then not do a scan...
        }
    }
}

void Controller::didSomething(QString strInfo)
{
    QDBusInterface remoteApp("net.appcheck.Proximus.UI", "/Proximus/UI",
                             "net.appcheck.Proximus.UI" , QDBusConnection::sessionBus());
    QDBusMessage PossibleError = remoteApp.call("updateLog",strInfo);
}

void Controller::itsMidnight()
{
    //0=sunday;6=saturday
    if (CurrentDayOfWeek < 6)
        CurrentDayOfWeek++;
    else
        CurrentDayOfWeek = 0;

    foreach(Rule* ruleStruct,  Rules) {
        if(ruleStruct->enabled && ruleStruct->data.weekdayRule.enabled){
            if(ruleStruct->data.weekdayRule.daysSelected.toStringList().contains(QString::number(CurrentDayOfWeek))){
                ruleStruct->data.weekdayRule.activated();
                didSomething("It's midnight. rule '" + ruleStruct->name + "'' DayOfWeek matched");
            }
            else{
                didSomething("It's midnight. rule " + ruleStruct->name + " dayOfWeek not matched");
                ruleStruct->data.weekdayRule.deactivated();
            }
        }
    }
}

void Controller::checkQueuedRules()
{

    foreach(Rule* ruleStruct,  Rules) {
        if (ruleStruct->waitingForWiFiScan){
            if (pendingScan){
                qDebug() << "Rule" << ruleStruct->name << "status check delayed (again) by pending wifi scan";
                didSomething("Rule " + ruleStruct->name + " status check delayed (again) by pending wifi scan");
                waitForScanTimer.stop();
                waitForScanTimer.start(300,360);//check again in 5-6 min
                waitForScanTimer.setSingleShot(true);
            }
            else {
                checkStatus(ruleStruct);
            }
        }
    }
}

void Controller::switchProfileNow()
{
    ProfileClient *profileClient = new ProfileClient(this);
    qDebug() << "really switching profile to" << lastRequestedProfile;
    didSomething("requesting profile switch to " + lastRequestedProfile);
    if (!profileClient->setProfile(lastRequestedProfile))
        qDebug() << "failed to switch profile!!";
    delete profileClient;
}

void Controller::chargingStateChanged(QSystemBatteryInfo::ChargingState state)
{//this tends to fire 2-3 times as the phone negotiates 100 to 500 ma
    qDebug() << "charging state changed to " << state;
    if (state == QSystemBatteryInfo::NotCharging){
        deviceIsCharging = false;
        didSomething("phone is not charging");

    }
    else if (state == QSystemBatteryInfo::Charging){
        deviceIsCharging = true;
        didSomething("phone is charging");
    }

    foreach(Rule* checkRule, Rules){
        if (checkRule->data.chargingRule.enabled){
            bool shouldActivate = false;
            if (deviceIsCharging && !checkRule->data.chargingRule.inverseCond)
                shouldActivate = true;
            if (!deviceIsCharging && checkRule->data.chargingRule.inverseCond)
                shouldActivate = true;
            if (shouldActivate)
                checkRule->data.chargingRule.activated();
            else
                checkRule->data.chargingRule.deactivated();
            qDebug() << "adj charging status of " << checkRule->name << "to" << shouldActivate;            
        }
    }    
}


Controller::~Controller()
{
    delete settings;
    delete fswatcher;
    delete calTimer;
    delete wifiTimer;
    delete satelliteInfoSource;
    delete myCalWrapper;
//    delete myBatteryInfo;
//    delete externalTimer;
}

void Controller::rulesStorageChanged() {
    MeeGo::QmHeartbeat *heartbeat;
    heartbeat = new MeeGo::QmHeartbeat();
    heartbeat->open(MeeGo::QmHeartbeat::SignalNeeded);
    QTime result = heartbeat->wait(2,15,MeeGo::QmHeartbeat::WaitHeartbeat);//wait a few secs to avoid needless redoing of this step and align to heartbeat
    heartbeat->close();
    delete heartbeat;
    qDebug() << "Slept " << result.second() << "seconds" ;

    settings->sync();//REALLY needed, first! (maybe not after i added the wait above?)
//    qDebug() << "rulesStorageChanged()" << settings->allKeys();
    //if service is supposed to be disabled, just exit
    if (settings->value("/settings/Service/enabled",true).toBool() == false){
        qDebug() << "service supposed to be disabled, exiting";
        didSomething("service supposed to be disabled, exiting");
        exit(0);
    }

    if (settings->value("/settings/Positioning/enabled",false).toBool()){
        qDebug() << "positioning allowed";
        startGPS();//since settings could have changed, restart GPS to set the correct positioning method
    }
    else
    {
        if (locationDataSource){//was already started
            locationDataSource->stopUpdates();
            locationDataSource = 0;
        }
    }

    //stop any existing wifi scan timer because we don't know if we need it anymore and will recreate it below
    wifiTimer->stop();

    //setup memory structure used to keep track of rules being active or not

    //as this DOES NOT happen often, it's okay to recreate from scratch
    //delete them properly before losing the references though

    foreach(Rule* deadRule, Rules){
        qDebug() << "killing " << deadRule->name;
        delete deadRule;
    }

    Rules.clear();

    //fill list
    int ruleNumber = 0;//counter
    settings->beginGroup("rules");
    Q_FOREACH(const QString &strRuleName, settings->childGroups()){//for each rule
        ruleNumber++;
        settings->beginGroup(strRuleName);
        if (settings->value("enabled").toBool() == true){//if enabled
            Rule* newRule = new Rule();
            newRule->name = strRuleName;
            qDebug() << "loaded rule" << strRuleName;
            Rules.insert(settings->value("Number",ruleNumber + 100).toInt(), newRule);
            newRule->enabled = true;
            DataLocation* ptrRuleDataLoc = new DataLocation;
            ptrRuleDataLoc->setParent(newRule);
            newRule->data.locationRule = ptrRuleDataLoc;
            ptrRuleDataLoc->active = false;//we can default the status to false, it will be re-evaluated within a minute
            ptrRuleDataLoc->enabled = settings->value("Location/enabled",false).toBool();
            ptrRuleDataLoc->inverseCond = settings->value("Location/NOT",false).toBool();
            ptrRuleDataLoc->radius = settings->value("Location/RADIUS").toInt();
            ptrRuleDataLoc->location.setLongitude(settings->value("Location/LONGITUDE").toDouble());
            ptrRuleDataLoc->location.setLatitude(settings->value("Location/LATITUDE").toDouble());
            if (ptrRuleDataLoc->enabled && locationDataSource)
            {
                ptrRuleDataLoc->areaMon = initAreaMonitor(ptrRuleDataLoc);
            }
            newRule->data.timeRule.setParent(newRule);
            newRule->data.timeRule.active = false;
            newRule->data.timeRule.enabled = settings->value("Time/enabled",false).toBool();
            newRule->data.timeRule.inverseCond = settings->value("Time/NOT",false).toBool();
            newRule->data.timeRule.time1 = settings->value("Time/TIME1").toTime();
            newRule->data.timeRule.time2 = settings->value("Time/TIME2").toTime();
            qDebug() << "time1/time2" << newRule->data.timeRule.time1 << newRule->data.timeRule.time2 ;
            //time rule is somewhat simple, we can do it here
            if (newRule->data.timeRule.enabled)
            {
                qint32 startTimeDiff, endTimeDiff;
                startTimeDiff = QTime::currentTime().secsTo(newRule->data.timeRule.time1);
                endTimeDiff = QTime::currentTime().secsTo(newRule->data.timeRule.time2);                
                if (startTimeDiff < 1)//can be negative if it occured < 12 hrs ago?
                    startTimeDiff = 86400 + startTimeDiff;// if was negative, that's not useful. we take the # seconds in a day (86,400) and subtract the # of seconds ago the event started to get the # of seconds when it starts next.
                if (endTimeDiff < 1)
                    endTimeDiff = 86400 + endTimeDiff;//same thing here
                qDebug()<< "adj. start/end timediff: " << startTimeDiff << endTimeDiff;
                //connect signal to rule
                connect(&newRule->data.timeRule, SIGNAL(activeChanged(Rule*)),
                        this, SLOT(checkStatus(Rule*))
                );
                newRule->data.timeRule.deactivateTimer.start(endTimeDiff * 1000);
                newRule->data.timeRule.deactivateTimer.setSingleShot(true);
                if (endTimeDiff < startTimeDiff)//means we are activated right now
                    newRule->data.timeRule.active = true;
                else {
                    newRule->data.timeRule.activateTimer.start(startTimeDiff * 1000);//convert to ms
                    newRule->data.timeRule.activateTimer.setSingleShot(true);
                    if (newRule->data.timeRule.inverseCond == true) {//need to set directly
                        qDebug() << "time inverse - should be set active";
                    }
                }
            }

            newRule->data.calendarRule.setParent(newRule);
            newRule->data.calendarRule.active = false;
            newRule->data.calendarRule.enabled = settings->value("Calendar/enabled",false).toBool();
            newRule->data.calendarRule.inverseCond = settings->value("Calendar/NOT",false).toBool();
            newRule->data.calendarRule.keywords = settings->value("Calendar/KEYWORDS").toString();
            //DayOfWeek
            newRule->data.weekdayRule.setParent(newRule);
            connect(&newRule->data.weekdayRule, SIGNAL(activeChanged(Rule*)),
                    this, SLOT(checkStatus(Rule*))
                    );
            newRule->data.weekdayRule.enabled = settings->value("DaysOfWeek/enabled",false).toBool();
            newRule->data.weekdayRule.inverseCond = settings->value("DaysOfWeek/NOT",false).toBool();
            newRule->data.weekdayRule.daysSelected = settings->value("DaysOfWeek/INDEXES");
            //set to active now if it should be because this stuff is only rechecked at midnight
             if(newRule->data.weekdayRule.daysSelected.toStringList().contains(QString::number(CurrentDayOfWeek))){
                 qDebug() << "weekday true on load";
                 newRule->data.weekdayRule.active = true;
             }
             else{
                 qDebug() << "weekday false on load";
                 newRule->data.weekdayRule.active = false;
             }
            //CHARGING STATUS
            newRule->data.chargingRule.active = deviceIsCharging;
            newRule->data.chargingRule.setParent(newRule);
            newRule->data.chargingRule.enabled = settings->value("Charging/enabled",false).toBool();
            newRule->data.chargingRule.inverseCond = settings->value("Charging/NOT",false).toBool();
            connect(&newRule->data.chargingRule, SIGNAL(activeChanged(Rule*)),
                    this, SLOT(checkStatus(Rule*))
                    );

            //WIFI
            newRule->data.wifiRule.active = false;
            newRule->data.wifiRule.setParent(newRule);
            newRule->data.wifiRule.enabled = settings->value("WiFi/enabled",false).toBool();
            newRule->data.wifiRule.inverseCond = settings->value("WiFi/NOT",false).toBool();

            //make sure we have an enabled timer if required by this rule
            if (newRule->data.wifiRule.enabled){
                settings->endGroup();
                settings->endGroup();//i thought if you start the value with a slash it would just work. nope.
                qDebug() << "setting wifi timer for " << settings->value("/settings/WIFI/interval",9).toInt() * 60 << " seconds";
                wifiTimer->start(60 * settings->value("/settings/WIFI/interval",9).toInt() - 15,
                                 60 * settings->value("/settings/WIFI/interval",9).toInt() + 45);
                settings->beginGroup("rules");
                settings->beginGroup(strRuleName);
                newRule->data.wifiRule.SSIDs = settings->value("/WiFi/SSIDs","").toString();
                connect(&newRule->data.wifiRule, SIGNAL(activeChanged(Rule*)),
                        this, SLOT(checkStatus(Rule*))
                        );
                if (!pendingScan)
                    requestScan();
            }
            qDebug() << "trying to check rule" << newRule->name;
            checkStatus(newRule);
            qDebug() << "done checking rule" << newRule->name;

        }
        else{//rule was not enabled, we skipped all of the above

        }
        settings->endGroup();
    }
    settings->endGroup();
    updateCalendar(); //call from here so rule changes trigger this again
}

//triggered by a heartbeat timer object every 45 min or so,
//checks calendar for any keyword matches and sets more timers to change the rule to active when those become current.
//if this api made any sense, i could use signals too
//this also checks for upcoming midnight and will setup a timer for that
void Controller::updateCalendar()
{//ugh so this thing opens the current users calendar... root / developer = bad      
//    QOrganizerManager defaultManager(this); //provides access to system address book, calendar
   // qDebug() << "params: " << defaultManager.managerParameters().values().count(); // params:  0
    //qDebug() << "manager: " << defaultManager.managerUri(); // manager:  "qtorganizer:mkcal:"
   // QMap<QString, QString> parameters;
   // parameters["filename"] = "/home/user/.calendar/db"; // database??
    //QOrganizerManager userManager("",parameters);
    //get list of all upcoming calendar events
    if (!midnightTimer.isActive()) //no existing timer
    {
        //qint32 secsToMidnight = 10;//FOR TESTING ONLY
        qint32 secsToMidnight = QTime::currentTime().secsTo(QTime::fromString("23:59:59","hh:mm:ss"));
        if (secsToMidnight < 3600){ //midnight in next hour
            midnightTimer.setMinimumInterval(secsToMidnight);
            midnightTimer.setMaximumInterval(secsToMidnight + 60);
            midnightTimer.setSingleShot(true);
            midnightTimer.start();
            didSomething("It's midnight in " + QString::number(secsToMidnight) + "s");
        }
    }

    QList<QOrganizerItem> entries =
             defaultManager.items(QDateTime::currentDateTime(),//not sure if this returns events already started
                                  QDateTime::currentDateTime().addSecs(3600)); //read next hour of calendar data
    //for each calendar rule; because they are in a QMap keyed by "number" it should be in order by the number
    Q_FOREACH(Rule* ruleStruct,  Rules) {
        bool foundMatch = false;
        QString keywords = ruleStruct->data.calendarRule.keywords;
        //seperate keywords string into list of keywords
        QStringList keywordList = keywords.split(" ");
        qDebug() << "found # keywords: " << keywordList.count();
        //then loop through all the upcoming calendar events
        qDebug() << "searching " << entries.count() << "calendar entries for the next hour";
        didSomething("checking next hour of calendar data (" +  QString::number(entries.count()) + " items) for keywords");
        Q_FOREACH(QOrganizerItem orgItem, entries){
            //and each individual keyword
            Q_FOREACH(QString keyword, keywordList){
                qDebug() << "check for keyword " << keyword;
                if (orgItem.displayLabel().contains(keyword, Qt::CaseInsensitive) || orgItem.description().contains(keyword, Qt::CaseInsensitive)) {
                    //keyword match, set up timer to activate this rule
                    foundMatch = true;
                    qDebug() << "keyword match";
                    //find seconds until event
                    qint16 startTimeDiff, endTimeDiff;
                    QOrganizerEventTime eventTime = orgItem.detail<QOrganizerEventTime>();
                    startTimeDiff = QDateTime::currentDateTime().secsTo(eventTime.startDateTime());
                    endTimeDiff = QDateTime::currentDateTime().secsTo(eventTime.endDateTime());
                    if ((startTimeDiff < 1) && (endTimeDiff > 1)) //already started, but not ended yet or all day event
                    {
                        #ifdef Q_WS_SIMULATOR
                        ruleStruct->data.calendarRule.activateTimer.start(20 * 1000);//timer is in ms
                        ruleStruct->data.calendarRule.activateTimer.setSingleShot(true);
                        ruleStruct->data.calendarRule.deactivateTimer.start(40 * 1000);
                        ruleStruct->data.calendarRule.deactivateTimer.setSingleShot(true);
                        connect(&ruleStruct->data.calendarRule.activateTimer, SIGNAL(timeout()),
                                &ruleStruct->data.calendarRule, SLOT(activated()));
                        connect(&ruleStruct->data.calendarRule.deactivateTimer, SIGNAL(timeout()),
                                &ruleStruct->data.calendarRule, SLOT(deactivated()));
                        #else
                        ruleStruct->data.calendarRule.activated();
                        ruleStruct->data.calendarRule.deactivateTimer.start(endTimeDiff * 1000);
                        ruleStruct->data.calendarRule.deactivateTimer.setSingleShot(true);
                        connect(&ruleStruct->data.calendarRule.deactivateTimer, SIGNAL(timeout()),
                                &ruleStruct->data.calendarRule, SLOT(deactivated()));
                        //connect the signals emited from those slots back to this class
                        connect(&ruleStruct->data.calendarRule, SIGNAL(activeChanged(Rule*)),
                                this, SLOT(checkStatus(Rule*)));
                        #endif
                    }
                    else if (startTimeDiff > 1) //starts in future
                    {
                        //ui->txtLog->appendPlainText("start in " + QString::number(startTimeDiff));
                        ruleStruct->data.calendarRule.activateTimer.start(startTimeDiff * 1000);//timer is in ms
                        ruleStruct->data.calendarRule.activateTimer.setSingleShot(true);
                        ruleStruct->data.calendarRule.deactivateTimer.start(endTimeDiff * 1000);
                        ruleStruct->data.calendarRule.deactivateTimer.setSingleShot(true);
                        connect(&ruleStruct->data.calendarRule.activateTimer, SIGNAL(timeout()),
                                &ruleStruct->data.calendarRule, SLOT(activated()));
                        connect(&ruleStruct->data.calendarRule.deactivateTimer, SIGNAL(timeout()),
                                &ruleStruct->data.calendarRule, SLOT(deactivated()));
                        //connect the signals emited from those slots back to this class
                        connect(&ruleStruct->data.calendarRule, SIGNAL(activeChanged(Rule*)),
                                this, SLOT(checkStatus(Rule*))
                                );
                    }
                }
                //if keywords not found but rule is inversed, we still need to set to active (now)
                if (foundMatch == false && ruleStruct->data.calendarRule.inverseCond == true)
                {//need to set directly (or we could have called deactivated(). whatever.)
                     ruleStruct->data.calendarRule.active = true;
                    // checkStatus(ruleStruct); //defer this
                }
            }
        }
    }
}

//sets calendar rule to active
void DataCalendar::activated()
{
    qDebug() << "calendar activated";
    if (this->inverseCond == false)
        this->active = true;
    else
        this->active = false;
    Q_EMIT activeChanged((Rule*)this->parent());
}

//sets calendar rule to inactive
void DataCalendar::deactivated()
{
    qDebug() << "calendar deactivated";
    if (this->inverseCond == false)
        this->active = false;
    else
        this->active = true;
    Q_EMIT activeChanged((Rule*)this->parent());
}

//sets time rule to active
void DataTime::activated()
{
    qDebug() << "time activated";
    if (this->inverseCond == false)
        this->active = true;
    else
        this->active = false;
    Q_EMIT activeChanged((Rule*)this->parent());
}

//sets time rule to inactive
void DataTime::deactivated()
{
    qDebug() << "time deactivated";
    if (this->inverseCond == false)
        this->active = false;
    else
        this->active = true;
    Q_EMIT activeChanged((Rule*)this->parent());
}

//sets wifi rule to active
void DataWifi::activated()
{
    qDebug() << "wifi activated";
    if (this->inverseCond == false)
        this->active = true;
    else
        this->active = false;
    Q_EMIT activeChanged((Rule*)this->parent());
}

//sets wifi rule to inactive
void DataWifi::deactivated()
{
    qDebug() << "wifi deactivated";
    if (this->inverseCond == false)
        this->active = false;
    else
        this->active = true;
    Q_EMIT activeChanged((Rule*)this->parent());
}

void Controller::startGPS()
{
    // Obtain the location data source if it is not obtained already
    if (!locationDataSource){
        locationDataSource = QGeoPositionInfoSource::createDefaultSource(this);
        if (!locationDataSource){
            // Not able to obtain the location data source
            qDebug() << "GPS FAILURE";
            didSomething("GPS FAILURE");
            return;
            }
        didSomething("Positioning started");
    }


    if (settings->value("/settings/GPS/enabled",false).toBool()) {
        if (locationDataSource->preferredPositioningMethods() != QGeoPositionInfoSource::AllPositioningMethods)
            locationDataSource->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        else
            qDebug() << "skipped gps method set";
        qDebug() << "gps on";
        didSomething("GPS enabled");
    }
    else {
        if (locationDataSource->preferredPositioningMethods() != QGeoPositionInfoSource::NonSatellitePositioningMethods)
            locationDataSource->setPreferredPositioningMethods(QGeoPositionInfoSource::NonSatellitePositioningMethods);
        else
            qDebug() << "skipped gps method set";
        qDebug() << "gps off";
        didSomething("GPS disabled");
    }
    locationDataSource->setUpdateInterval(settings->value("/settings/GPS/enabled",60).toInt() * 1000);

    // Start listening for position updates
    locationDataSource->startUpdates();
}

//called any time we activate / deactivate a rule setting
//this just does some boolean math to check if the whole rule is now active or inactive
void Controller::checkStatus(Rule* ruleStruct)
{
    if (pendingScan){
        qDebug() << "Rule" << ruleStruct->name << "status check delayed by pending wifi scan";
        didSomething("Rule " + ruleStruct->name + " status check delayed by pending wifi scan");

        waitForScanTimer.stop();
        waitForScanTimer.start(14,18);
        waitForScanTimer.setSingleShot(true);
        ruleStruct->waitingForWiFiScan = true;
        return;
    }
    //pendingRuleQueue.enqueue(ruleStruct); //still, enqueue this item so we don't have to check if empty here
    qDebug() << "now checking " << ruleStruct->name;



    QVariant locationCond = false;
    if (ruleStruct->data.locationRule->enabled && locationDataSource) //no locationDataSource if user has disabled positioning
    {
        if (ruleStruct->data.locationRule->active)
            locationCond = true;
        else
            locationCond = false;
        if (ruleStruct->data.locationRule->inverseCond)
            locationCond = !locationCond.toBool();
    }
    else
        locationCond = true;

    QVariant timeCond = false;
    if (ruleStruct->data.timeRule.enabled)
    {
        if (ruleStruct->data.timeRule.active)
            timeCond = true;
        else
            timeCond = false;
        if (ruleStruct->data.timeRule.inverseCond)
            timeCond = !timeCond.toBool();
    }
    else
        timeCond = true;

    QVariant calendarCond = false;
    if (ruleStruct->data.calendarRule.enabled)
    {
        if (ruleStruct->data.calendarRule.active)
            calendarCond = true;
        else
            calendarCond = false;
        if (ruleStruct->data.calendarRule.inverseCond)
            calendarCond = !calendarCond.toBool();
    }
    else
        calendarCond = true;

    QVariant dayOfWeekCond = false;
    if (ruleStruct->data.weekdayRule.enabled){
        if(ruleStruct->data.weekdayRule.active)
            dayOfWeekCond = true;
        else
            dayOfWeekCond = false;
        if(ruleStruct->data.weekdayRule.inverseCond)
            dayOfWeekCond = !dayOfWeekCond.toBool();
    }
    else
        dayOfWeekCond = true;

    QVariant wifiCond = false;
    if (ruleStruct->data.wifiRule.enabled){
        if (ruleStruct->data.wifiRule.active)
            wifiCond = true;
        else
            wifiCond = false;
        if(ruleStruct->data.wifiRule.inverseCond)
            wifiCond = !wifiCond.toBool();
    }
    else
        wifiCond = true;

    QVariant chargingCond = false;
    if (ruleStruct->data.chargingRule.enabled){
        if (ruleStruct->data.chargingRule.active)
            chargingCond = true;
        else
            chargingCond = false;
        if(ruleStruct->data.chargingRule.inverseCond)
            chargingCond = !chargingCond.toBool();
    }
    else
        chargingCond = true;

    QVariant result = locationCond.toBool() && timeCond.toBool() && calendarCond.toBool() && dayOfWeekCond.toBool() && wifiCond.toBool() && chargingCond.toBool();
    didSomething(ruleStruct->name + "="+ result.toString() +"(loc:" + locationCond.toString() + " time:" + timeCond.toString() + " cal:" + calendarCond.toString() + " day:" + dayOfWeekCond.toString() + " wifi:"+ wifiCond.toString() + " charging:"+ chargingCond.toString() +")") ;
    qDebug() << ruleStruct->name << "=" << result  << "(loc:" << locationCond.toString() << " time:" << timeCond.toString() << " cal:" << calendarCond.toString() << " day:" << dayOfWeekCond.toString() << " wifi:" << wifiCond.toString()  << " charging:" << chargingCond.toString() << ")";
    ruleStruct->active = result.toBool();

    bool backOutSettings = false;
    if(!settings->group().contains(ruleStruct->name)){
        backOutSettings = true;
        settings->beginGroup("rules");
        settings->beginGroup(ruleStruct->name);
    }
    if (result.toBool())
    {//rule evaluated to true
        qDebug() << settings->value("Actions/Profile/enabled",999);
        if (settings->value("Actions/Profile/enabled",false).toBool() == true)
        {//set profile first to enable / disable sounds during other actions
            qDebug() << "queuing profile switch to" << settings->value("Actions/Profile/NAME","").toString();
            didSomething("queuing profile switch to " + settings->value("Actions/Profile/NAME","").toString());
            lastRequestedProfile = settings->value("Actions/Profile/NAME","").toString();
            profileSwitchTimer.setSingleShot(true);
            profileSwitchTimer.start(2,5);
        }
        if (settings->value("Actions/Reminder/enabled",false).toBool() == true) {
            //check if this action was done in the last 12 hours. secsto will be negative or zero if not
            if (QDateTime::currentDateTime().secsTo(settings->value("Actions/Reminder/DisableUntil",QDateTime::currentDateTime()).toDateTime()) < 60){

            //I don't think the process watching for these alarms will catch it in time if you set it only a few seconds away. It didn't work in my initial testing anyways.
            myCalWrapper->setAlarm(QDateTime::currentDateTime().addSecs(70), QDateTime::currentDateTime().addSecs(130),settings->value("Actions/Reminder/TEXT","unknown").toString());
            //disable alarm for 12 hours, i will make this configurable later
            settings->setValue("Actions/Reminder/DisableUntil",QDateTime::currentDateTime().addSecs(3600 * 12));
            }
            else{
                qDebug() << "skipping create new reminder, disabled until " << settings->value("Actions/Reminder/DisableUntil",QDateTime::currentDateTime()).toDateTime();
                qDebug() << "code got now.secsto as " << QDateTime::currentDateTime().secsTo(settings->value("Actions/Reminder/DisableUntil",QDateTime::currentDateTime()).toDateTime());
                didSomething("skipping create new reminder, disabled until " + (settings->value("Actions/Reminder/DisableUntil",QDateTime::currentDateTime()).toDateTime()).toString()  );
            }
        }
        if (settings->value("Actions/PowerSave/enabled",false).toBool() == true) {
            //force power save mode on
            MeeGo::QmDeviceMode deviceMode;
            deviceMode.setPSMState (MeeGo::QmDeviceMode::PSMStateOn);
            qDebug() << "tried to set PSM on";
            didSomething("PSM on");
        }

        if (settings->value("/Actions/RunCommand/enabled",false).toBool() == true) {
            if (!ruleCommandAlreadyStarted.contains(ruleStruct->name) || //if this rule isn't in qmap
                    !ruleCommandAlreadyStarted.value(ruleStruct->name)){ //or if this rule is false in qmap
                    QProcess newProcess; //start the process
                    newProcess.startDetached(settings->value("/Actions/RunCommand/TEXT","").toString());
                    ruleCommandAlreadyStarted.insert(ruleStruct->name,true); //and update qmap to not run it again
                    didSomething("Starting process " + settings->value("/Actions/RunCommand/TEXT","").toString());
                }
        }

    }
    else//rule evaluated to false
    {
        if (settings->value("Actions/PowerSave/enabled",false).toBool() == true) {
            //force power save mode off once rule is inactive
            MeeGo::QmDeviceMode deviceMode;
            deviceMode.setPSMState (MeeGo::QmDeviceMode::PSMStateOff);
            qDebug() << "tried to set PSM off";
            didSomething("PSM off");
        }
        if (ruleCommandAlreadyStarted.contains(ruleStruct->name))
            ruleCommandAlreadyStarted.insert(ruleStruct->name,false);//update qmap to run process next time rule is true
    }
    if(backOutSettings){
        settings->endGroup();
        settings->endGroup();
    }
}

//create and return a (pointer to) a single QGeoAreaMonitor
QGeoAreaMonitor * Controller::initAreaMonitor(DataLocation *& Dataloc)
{
    // Create the area monitor
    qDebug() << "started create area monitor";
    QGeoAreaMonitor *monitor = QGeoAreaMonitor::createDefaultMonitor(Dataloc);
    if (monitor == NULL){
        qDebug() << "error - failed to create monitor";
         return NULL;
    }

    // Connect the area monitoring signals to the corresponding slots
    if (!connect(monitor, SIGNAL(areaEntered(QGeoPositionInfo)),
             Dataloc, SLOT(areaEntered(QGeoPositionInfo))))
        //QMessageBox::critical(this,"error","error connecting slots");
        qDebug() << "error connecting slots";

    connect(monitor, SIGNAL(areaExited(QGeoPositionInfo)),
            Dataloc, SLOT(areaExited(QGeoPositionInfo)));

    //connect the signals emited from those slots back to this class
    connect(Dataloc, SIGNAL(activeChanged(Rule*)),
            this, SLOT(checkStatus(Rule*))
            );

    monitor->setCenter(Dataloc->location);
    monitor->setRadius(Dataloc->radius);
    return monitor;
}

void DataLocation::areaEntered(const QGeoPositionInfo &update) {
    // The area has been entered
    qDebug() << "area entered";
    if (this->inverseCond == false)
        this->active = true;
    else
        this->active = false;
    Q_EMIT activeChanged((Rule*)this->parent());
}

void DataLocation::areaExited(const QGeoPositionInfo &update) {
    // The area has been exited
    qDebug() << "area exited";
     if (this->inverseCond == false)
         this->active = false;
     else
         this->active = true;
     Q_EMIT activeChanged((Rule*)this->parent());
}

Rule::Rule()
{
    waitingForWiFiScan = false;
}

Rule::~Rule()
{
}

RuleData::RuleData()
    :locationRule(new DataLocation)
{
}
RuleData::~RuleData()
{
    if (locationRule)
        delete locationRule;
}

DataLocation::DataLocation()
{
}
DataLocation::~DataLocation()
{
    if (areaMon)
    {
        delete areaMon;
    }
}

void DataDayOfWeek::activated()
{
    qDebug() << "DayOfWeek activated";
    if (this->inverseCond == false)
        this->active = true;
    else
        this->active = false;
    Q_EMIT activeChanged((Rule*)this->parent());
}

void DataDayOfWeek::deactivated()
{
    qDebug() << "DayOfWeek deactivated";
    if (this->inverseCond == false)
        this->active = false;
    else
        this->active = true;
    Q_EMIT activeChanged((Rule*)this->parent());
}

void DataCharging::activated()
{
    if (this->inverseCond == false)
        this->active = true;
    else
        this->active = false;
    Q_EMIT activeChanged((Rule*)this->parent());
}

void DataCharging::deactivated()
{
    if (this->inverseCond == false)
        this->active = false;
    else
        this->active = true;
    Q_EMIT activeChanged((Rule*)this->parent());
}
