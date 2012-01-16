#include "controller.h"
#include "profileclient.h"
#include "wlancond-dbus.h"
#include "wlancond.h"

Controller::Controller(QObject *parent) :
    QObject(parent), settings(new QSettings("/home/user/.config/FakeCompany/Proximus.conf",QSettings::NativeFormat,this))
    ,fswatcher(new QFileSystemWatcher(this))
    ,calTimer(new QSystemAlignedTimer(this))
    ,wifiTimer(new QSystemAlignedTimer(this))

{//important to init qsettings like that so it doesn't store in /home/root/ or whatever other account name
    qDebug() << "starting proximus";
    pendingScan = false;
    fswatcher->addPath("/home/user/.config/FakeCompany/Proximus.conf");
    connect(fswatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(rulesStorageChanged()));//don't need the filename passed
    QCoreApplication::setOrganizationName("FakeCompany");
    QCoreApplication::setOrganizationDomain("appcheck.net");
    QCoreApplication::setApplicationName("Proximus");
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
        #endif
    #endif

    QDBusConnection::systemBus().connect(WLANCOND_SERVICE, WLANCOND_SIG_PATH,
                                         WLANCOND_SIG_INTERFACE, WLANCOND_SCAN_RESULTS_SIG,
                                         this, SLOT(recvScan(const QDBusMessage&)));

    connect(&waitAndReScanTimer, SIGNAL(timeout()),
            this, SLOT(requestScan()));

    qDebug() << "init complete";
}

void Controller::recvScan(const QDBusMessage &msg)
{//i'd never have figured this out without http://b.zekjur.net/post/27
    nearbySSIDs.clear();
    QList<QVariant> args = msg.arguments();
    int networks = args.value(0).value<int>();
    for (int c = 0; c < networks; c++) {
        int pos = 1 + (c * 5);
        nearbySSIDs.append(args.value(pos).value<QByteArray>());
    }
    qDebug() << nearbySSIDs;
    //check all rules now
    bool match;
    pendingScan = false;
    foreach(Rule* ruleStruct,  Rules) {
        match = false;
        if(ruleStruct->enabled && ruleStruct->data.wifiRule.enabled)
        {
            QStringList RuleSSIDs = ruleStruct->data.wifiRule.SSIDs.split(" ");
            foreach(QString SSIDfromScan, nearbySSIDs)
            {
                foreach(QString ruleSSID,RuleSSIDs)
                {
                    if(ruleSSID.toUpper() == SSIDfromScan.toUpper())
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
    pendingScan = true;
    QDBusInterface remoteApp(WLANCOND_SERVICE, WLANCOND_REQ_PATH,
                              WLANCOND_REQ_INTERFACE , QDBusConnection::systemBus());

    QDBusMessage reply = remoteApp.call(WLANCOND_SCAN_REQ, WLANCOND_TX_POWER10, QByteArray (""), 0);
    if (reply.type() == QDBusMessage::ErrorMessage){
        if  (reply.errorMessage() == "com.nokia.wlancond.error.already_active"){
            //wait and rescan
            qDebug() << "stupid thing is busy";
            waitAndReScanTimer.start(7,13);
            waitAndReScanTimer.setSingleShot(true);
        }
        else
            qDebug() << reply.errorMessage(); //likes to get "com.nokia.wlancond.error.already_active" , and then not do a scan...
    }
}


Controller::~Controller()
{
    delete settings;
    delete fswatcher;
    delete calTimer;
    delete satelliteInfoSource;
}

void Controller::rulesStorageChanged() {
    settings->sync();//REALLY needed, first!
//    qDebug() << "rulesStorageChanged()" << settings->allKeys();
    //if service is supposed to be disabled, just exit
    if (settings->value("/settings/Service/enabled",true).toBool() == false){
        qDebug() << "service supposed to be disabled, exiting";
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
    settings->beginGroup("rules");
    Q_FOREACH(const QString &strRuleName, settings->childGroups()){//for each rule
        settings->beginGroup(strRuleName);
        if (settings->value("enabled").toBool() == true){//if enabled
            Rule* newRule = new Rule();
            newRule->name = strRuleName;
            qDebug() << "loaded rule" << strRuleName;
            Rules.insert(strRuleName,newRule);
            newRule->enabled = true;
            DataLocation* ptrRuleDataLoc = new DataLocation;
            ptrRuleDataLoc->setParent(newRule);
            newRule->data.locationRule = ptrRuleDataLoc;
            ptrRuleDataLoc->active = false;//we can default the status to false, it will be re-evaluated within a minute
            ptrRuleDataLoc->enabled = settings->value("Location/enabled").toBool();
            ptrRuleDataLoc->inverseCond = settings->value("Location/NOT").toBool();
            ptrRuleDataLoc->radius = settings->value("Location/RADIUS").toInt();
            ptrRuleDataLoc->location.setLongitude(settings->value("Location/LONGITUDE").toDouble());
            ptrRuleDataLoc->location.setLatitude(settings->value("Location/LATITUDE").toDouble());
            if (ptrRuleDataLoc->enabled)
            {
                ptrRuleDataLoc->areaMon = initAreaMonitor(ptrRuleDataLoc);
            }
            newRule->data.timeRule.setParent(newRule);
            newRule->data.timeRule.active = false;
            newRule->data.timeRule.enabled = settings->value("Time/enabled").toBool();
            newRule->data.timeRule.inverseCond = settings->value("Time/NOT").toBool();
            newRule->data.timeRule.time1 = settings->value("Time/TIME1").toTime();
            newRule->data.timeRule.time2 = settings->value("Time/TIME2").toTime();
            qDebug() << "time1/time2" << newRule->data.timeRule.time1 << newRule->data.timeRule.time2 ;
            //time rule is somewhat simple, we can do it here
            if (newRule->data.timeRule.enabled)
            {
                qint32 startTimeDiff, endTimeDiff;
                startTimeDiff = QTime::currentTime().secsTo(newRule->data.timeRule.time1);
                endTimeDiff = QTime::currentTime().secsTo(newRule->data.timeRule.time2);
                qDebug()<< "start/end timediff: " << startTimeDiff << endTimeDiff; //27240 //34440
                if (startTimeDiff < 1)//can be negative if it occured < 12 hrs ago?
                    startTimeDiff = 86400 + startTimeDiff;// if was negative, that's not useful. we take the # seconds in a day (86,400) and subtract the # of seconds ago the event started to get the # of seconds when it starts next.
                if (endTimeDiff < 1)
                    endTimeDiff = 86400 + endTimeDiff;//same thing here
                //connect signal to rule
                connect(&newRule->data.timeRule, SIGNAL(activeChanged(Rule*)),
                        this, SLOT(checkStatus(Rule*))
                );
                newRule->data.timeRule.deactivateTimer.start(endTimeDiff * 1000);
                newRule->data.timeRule.deactivateTimer.setSingleShot(true);
                //ui->txtLog->appendPlainText("timer to deactivate rule set for " + QString::number(endTimeDiff) + "s");
                if (endTimeDiff < startTimeDiff)//means we are activated right now
                    newRule->data.timeRule.activated();
                else {
                    newRule->data.timeRule.activateTimer.start(startTimeDiff * 1000);//convert to ms
                    newRule->data.timeRule.activateTimer.setSingleShot(true);
                   // ui->txtLog->appendPlainText("timer to activate rule set for " +  QString::number(startTimeDiff) + "s");
                    if (newRule->data.timeRule.inverseCond == true) {//need to set directly
                        qDebug() << "time inverse - should be set active";
                        //newRule->data.timeRule.active = true;
                        checkStatus(newRule);
                    }
                }
            }

            newRule->data.calendarRule.setParent(newRule);
            newRule->data.calendarRule.active = false;
            newRule->data.calendarRule.enabled = settings->value("Calendar/enabled").toBool();
            newRule->data.calendarRule.inverseCond = settings->value("Calendar/NOT").toBool();
            newRule->data.calendarRule.keywords = settings->value("Calendar/KEYWORDS").toString();
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

        }
        else{//rule was not enabled, we skipped all of the above
          //  ui->listWidgetRules->item(ui->listWidgetRules->count() - 1)->setForeground(Qt::red);
        }
        settings->endGroup();
    }
    settings->endGroup();
    updateCalendar(); //call from here so rule changes trigger this again
}

//triggered by a heartbeat timer object every 45 min or so,
//checks calendar for any keyword matches and sets more timers to change the rule to active when those become current.
//if this api made any sense, i could use signals too
void Controller::updateCalendar()
{//ugh so this thing opens the current users calendar... root / developer = bad
//    QOrganizerManager defaultManager(this); //provides access to system address book, calendar
   // qDebug() << "params: " << defaultManager.managerParameters().values().count(); // params:  0
    //qDebug() << "manager: " << defaultManager.managerUri(); // manager:  "qtorganizer:mkcal:"
   // QMap<QString, QString> parameters;
   // parameters["filename"] = "/home/user/.calendar/db"; // database??
    //QOrganizerManager userManager("",parameters);
    //get list of all upcoming calendar events
    QList<QOrganizerItem> entries =
             defaultManager.items(QDateTime::currentDateTime(),//not sure if this returns events already started
                                  QDateTime::currentDateTime().addSecs(3600)); //read next hour of calendar data
    //for each calendar rule
    Q_FOREACH(Rule* ruleStruct,  Rules) {
        bool foundMatch = false;
        QString keywords = ruleStruct->data.calendarRule.keywords;
        //seperate keywords string into list of keywords
        QStringList keywordList = keywords.split(" ");
        qDebug() << "found # keywords: " << keywordList.count();
        //then loop through all the upcoming calendar events
        qDebug() << "searching " << entries.count() << "calendar entries for the next hour";
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
                {//need to set directly
                     ruleStruct->data.calendarRule.active = true;
                     checkStatus(ruleStruct);
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

void DataWifi::pretendChanged()
{
    qDebug() << "pick me!";
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
            return;
            }
    }


    if (settings->value("/settings/GPS/enabled",false).toBool()) {
        if (locationDataSource->preferredPositioningMethods() != QGeoPositionInfoSource::AllPositioningMethods)
            locationDataSource->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        else
            qDebug() << "skipped gps method set";
        qDebug() << "gps on";
    }
    else {
        if (locationDataSource->preferredPositioningMethods() != QGeoPositionInfoSource::NonSatellitePositioningMethods)
            locationDataSource->setPreferredPositioningMethods(QGeoPositionInfoSource::NonSatellitePositioningMethods);
        else
            qDebug() << "skipped gps method set";
        qDebug() << "gps off";
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
        qDebug()  << "waiting for scan";
        connect(&ruleStruct->waitForScanTimer, SIGNAL(timeout()),
                &ruleStruct->data.wifiRule, SLOT(pretendChanged()));

        ruleStruct->waitForScanTimer.start(14,18);
        ruleStruct->waitForScanTimer.setSingleShot(true);

        return;
    }

    disconnect(&ruleStruct->waitForScanTimer, SIGNAL(timeout()),
            &ruleStruct->data.wifiRule, SLOT(pretendChanged()));

    bool locationCond = false;
    if (ruleStruct->data.locationRule->enabled && locationDataSource) //no locationDataSource if user has disabled positioning
    {
        if (ruleStruct->data.locationRule->active)
            locationCond = true;
        else
            locationCond = false;
        if (ruleStruct->data.locationRule->inverseCond)
            locationCond = !locationCond;
    }
    else
        locationCond = true;

    bool timeCond = false;
    if (ruleStruct->data.timeRule.enabled)
    {
        if (ruleStruct->data.timeRule.active)
            timeCond = true;
        else
            timeCond = false;
        if (ruleStruct->data.timeRule.inverseCond)
            timeCond = !timeCond;
    }
    else
        timeCond = true;

    bool calendarCond = false;
    if (ruleStruct->data.calendarRule.enabled)
    {
        if (ruleStruct->data.calendarRule.active)
            calendarCond = true;
        else
            calendarCond = false;
        if (ruleStruct->data.calendarRule.inverseCond)
            calendarCond = !calendarCond;
    }
    else
        calendarCond = true;

    bool wifiCond = false;
    if (ruleStruct->data.wifiRule.enabled){
        if (ruleStruct->data.wifiRule.active)
            wifiCond = true;
        else
            wifiCond = false;
        if(ruleStruct->data.wifiRule.inverseCond)
            wifiCond = !wifiCond;
    }
    else
        wifiCond = true;

    qDebug() << "checkStatus() loc/time/cal/wifi " << ruleStruct->name << locationCond << timeCond << calendarCond << wifiCond;
    bool result = locationCond && timeCond && calendarCond && wifiCond;
    ruleStruct->active = result;

    //should write some info to the status tab

    if (result)
    {
        settings->beginGroup("rules");
        settings->beginGroup(ruleStruct->name);
        if (settings->value("Actions/Run/enabled",false).toBool() == true)
        {//run
            qDebug() << "supposed to run something";
        }
        if (settings->value("Actions/Profile/enabled",false).toBool() == true)
        {//set profile
            #ifndef Q_WS_SIMULATOR
            qDebug() << "attempting to switch to profile " << settings->value("Actions/Profile/NAME","").toString();
            ProfileClient *profileClient = new ProfileClient(this);
            if (!profileClient->setProfile(settings->value("Actions/Profile/NAME","").toString()) )
                qDebug() << "failed to switch profile!!";
            delete profileClient;
            #else

            #endif
        }
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
}

//Rule::checkMeNow()
//{
//    this->data.wifiRule.pretendChanged();
//}

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
