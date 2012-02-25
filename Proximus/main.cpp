#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QCoreApplication>
#include <QDeclarativeComponent>
#include <MDeclarativeCache>
#include <main.h>
#include <profileclient.h>
#include <dbusiface.h>

MySettings::MySettings():
    qsettInternal(new QSettings("/home/user/.config/FakeCompany/Proximus.conf",QSettings::NativeFormat,this))
{
}
MySettings::~MySettings()
{
}

QString ProximusUtils::isServiceRunning()
{
    QProcess p;
    QString output;
    //needs root :(
    //p.start("/sbin/status apps/proximus-daemon");
    p.start("/bin/pidof proximus-daemon");
    p.waitForFinished(-1);
    output = p.readAllStandardOutput();
    qDebug() << output;
    qDebug() << "e:" + p.readAllStandardError();
    if (output.length() > 1)
        return "daemon appears to be running as pid " + output;
    else
        return "error - cannot find pid of daemon! May be normal if you just started the device.";
}

void ProximusUtils::refreshRulesModel()
{
    //rules_ptr->clear();
    myModel->clear();
    RuleMap.clear();
    MySettings tmpSettings;
    tmpSettings.beginGroup("rules");
    int counter = 0;
    bool needsReindex = false;
    foreach(const QString &strRuleName, tmpSettings.childGroups()){//for each rule
        counter++;
        tmpSettings.beginGroup(strRuleName);
        int FoundRuleNumber = tmpSettings.getValue("Number",counter + 100).toInt();
        if (FoundRuleNumber >= 100) needsReindex = true;
        RuleMap.insert(FoundRuleNumber,strRuleName);
        tmpSettings.endGroup();
    }
    if (needsReindex){//some rules didn't have rule # set, re-index rules
        qDebug() << "reindexing " << counter << " rules";
        counter = 0;
        QMap<int,QString> tempMap;
        foreach(QString strRuleName, RuleMap){
            tempMap.insert(counter++, strRuleName);
            tmpSettings.beginGroup(strRuleName);
            tmpSettings.setValue("Number",counter);
            tmpSettings.endGroup();
        }
        RuleMap = tempMap;
    }

    foreach(QString strRuleName, RuleMap){

        tmpSettings.beginGroup(strRuleName);
        myModel->append(new RuleObject(strRuleName,
                                       tmpSettings.getValue("enabled",false).toBool(),
                                       tmpSettings.getValue("Number",counter++ + 100).toInt()
                                       ));
        tmpSettings.endGroup();
    }

    tmpSettings.endGroup();//end rules
    view_ptr->rootContext()->setContextProperty("objRulesModel",(myModel));
}

void ProximusUtils::moveRuleUp(int rulenum)
{
    if (rulenum == 1)
        return;
    int i = rulenum;
    MySettings tmpSettings;
    tmpSettings.beginGroup("rules");
    tmpSettings.beginGroup(RuleMap[i]);
    tmpSettings.setValue("Number", i - 1);
    tmpSettings.endGroup();
    if (RuleMap.contains(i-1)){
        tmpSettings.beginGroup(RuleMap[i-1]);
        tmpSettings.setValue("Number", i);
        tmpSettings.endGroup();
    }
    tmpSettings.endGroup();
    refreshRulesModel();
}

void ProximusUtils::moveRuleDown(int rulenum)
{
    if (rulenum == RuleMap.count())
        return;
    int i = rulenum;
    MySettings tmpSettings;
    tmpSettings.beginGroup("rules");
    tmpSettings.beginGroup(RuleMap[i]);
    tmpSettings.setValue("Number", i + 1);
    tmpSettings.endGroup();
    if (RuleMap.contains(i+1)){
        tmpSettings.beginGroup(RuleMap[i+1]);
        tmpSettings.setValue("Number", i);
        tmpSettings.endGroup();
    }
    tmpSettings.endGroup();
    refreshRulesModel();
}

void ProximusUtils::deleteRule(int rulenum)
{
    MySettings tmpSettings;
    tmpSettings.beginGroup("rules");
    tmpSettings.remove(RuleMap[rulenum]);
    while (rulenum < RuleMap.count()){
        rulenum++;
        tmpSettings.beginGroup(RuleMap[rulenum]);
        tmpSettings.setValue("Number", rulenum - 1);
        tmpSettings.endGroup();
    }
    tmpSettings.endGroup();
    refreshRulesModel();
}

RuleObject::RuleObject(QString name, bool enabled, int number)
{
        strname = name;
        boolenabled = enabled;
        ruleNumber = number;
}

bool RuleObject::enabled()
{
    return boolenabled;
}

QString RuleObject::name()
{
    return strname;
}

int RuleObject::number()
{
    return ruleNumber;
}


void RuleObject::setEnabled(bool enabled)
{
    boolenabled = enabled;
    emit myModelChanged();
}

void RuleObject::setName(QString name)
{
    strname = name;
    emit myModelChanged();
}

void RuleObject::setNumber(int number)
{
    ruleNumber = number;
    emit myModelChanged();
}

RuleObject::~RuleObject()
{

}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("FakeCompany");
    QCoreApplication::setOrganizationDomain("appcheck.net");
    QCoreApplication::setApplicationName("Proximus");

    QApplication* app = MDeclarativeCache::qApplication(argc, argv);

    qmlRegisterType<ProximusLog>("net.appcheck.Proximus", 1, 0, "ProximusLog");

    //QDeclarativeEngine engine;
    //QDeclarativeComponent component(&engine, QUrl("qrc:StatusPage.qml"));
    //ProximusLog *objProximusLog = qobject_cast<ProximusLog *>(component.create());

    QSharedPointer<QDeclarativeView> view(MDeclarativeCache::qDeclarativeView());


    ProximusUtils objproximusUtils;
    ProximusLog objProximusLog;


    DbusIface dbusIface(&objProximusLog);
//    QObject::connect(&dbusIface, SIGNAL(newLogInfo(QVariant)),
//           &objProximusLog, SIGNAL(newLogInfo(QVariant)));

    QDBusConnection::sessionBus().registerObject("/Proximus/UI",&objProximusLog);
    QDBusConnection::sessionBus().registerService("net.appcheck.Proximus.UI");


    MySettings objSettings;
    QList<QObject*> rulesList;
    objproximusUtils.myModel = new QObjectListModel();
    objproximusUtils.myModel->setObjectList(rulesList);

    //objproximusUtils.rules_ptr = &rulesList;//set refs for later
    objproximusUtils.view_ptr = view;

//    objSettings.beginGroup("settings");
//    if (!objSettings.contains("GPS")) //first run, need to create default settings
//    {
//        objSettings.setValue("GPS/enabled",false);
//        objSettings.setValue("Service/enabled",true);
//    }
//    objSettings.endGroup();//end settings
    objSettings.beginGroup("rules");
    if (objSettings.childGroups().count() == 0) //first run, or no rules -- create two example rules
    {
        objSettings.setValue("Example Rule1/enabled",(bool)true);
        objSettings.setValue("Example Rule1/Location/enabled",(bool)true);
        objSettings.setValue("Example Rule1/Location/Number",(int)1);
        objSettings.setValue("Example Rule1/Location/NOT",(bool)false);
        objSettings.setValue("Example Rule1/Location/RADIUS",(double)250);
        objSettings.setValue("Example Rule1/Location/LONGITUDE",(double)-113.485336);
        objSettings.setValue("Example Rule1/Location/LATITUDE",(double)53.533064);

        objSettings.setValue("Example Rule2/enabled",(bool)false);
        objSettings.setValue("Example Rule2/Location/enabled",(bool)true);
        objSettings.setValue("Example Rule2/Location/Number",(int)2);
        objSettings.setValue("Example Rule2/Location/NOT",(bool)false);
        objSettings.setValue("Example Rule2/Location/RADIUS",(double)250);
        objSettings.setValue("Example Rule2/Location/LONGITUDE",(double)-113.485336);
        objSettings.setValue("Example Rule2/Location/LATITUDE",(double)53.533064);
    }        

//    Q_FOREACH(const QString &strRuleName, objSettings.childGroups()){//for each rule
//        objSettings.beginGroup(strRuleName);
//        rulesList.append(new RuleObject(strRuleName,objSettings.getValue("enabled",false).toBool()));
//        objSettings.endGroup();
//    }

    objSettings.endGroup();//end rules

    objproximusUtils.refreshRulesModel();

    ProfileClient *profileClient = new ProfileClient(NULL);

    view->rootContext()->setContextProperty("objProfileClient",profileClient);

    view->rootContext()->setContextProperty("objProximusLog",&objProximusLog);
    view->rootContext()->setContextProperty("objProximusUtils",&objproximusUtils);
    view->rootContext()->setContextProperty("objQSettings",&objSettings);
    //view->rootContext()->setContextProperty("objRulesModel", QVariant::fromValue(rulesList));

    view->setSource(MDeclarativeCache::applicationDirPath()
                    + QLatin1String("/../qml/Proximus/main.qml"));

    QObject::connect(view->engine(), SIGNAL(quit()), view.data(), SLOT(close()));

    view->showFullScreen();

    app->exec();

}
