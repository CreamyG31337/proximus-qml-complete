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
    rules_ptr->clear();
    MySettings tmpSettings;
    tmpSettings.beginGroup("rules");
    Q_FOREACH(const QString &strRuleName, tmpSettings.childGroups()){//for each rule
        tmpSettings.beginGroup(strRuleName);
        rules_ptr->append(new RuleObject(strRuleName,tmpSettings.getValue("enabled",false).toBool()));
        tmpSettings.endGroup();
    }
    tmpSettings.endGroup();//end rules
    view_ptr->rootContext()->setContextProperty("objRulesModel", QVariant::fromValue(*rules_ptr));
}

RuleObject::RuleObject(QString name, bool enabled)
{
        strname = name;
        boolenabled = enabled;
}

bool RuleObject::enabled()
{
    return boolenabled;
}

QString RuleObject::name()
{
    return strname;
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
    objproximusUtils.rules_ptr = &rulesList;//set refs for later
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
        objSettings.setValue("Example Rule1/Location/NOT",(bool)false);
        objSettings.setValue("Example Rule1/Location/RADIUS",(double)250);
        objSettings.setValue("Example Rule1/Location/LONGITUDE",(double)-113.485336);
        objSettings.setValue("Example Rule1/Location/LATITUDE",(double)53.533064);

        objSettings.setValue("Example Rule2/enabled",(bool)false);
        objSettings.setValue("Example Rule2/Location/enabled",(bool)true);
        objSettings.setValue("Example Rule2/Location/NOT",(bool)false);
        objSettings.setValue("Example Rule2/Location/RADIUS",(double)250);
        objSettings.setValue("Example Rule2/Location/LONGITUDE",(double)-113.485336);
        objSettings.setValue("Example Rule2/Location/LATITUDE",(double)53.533064);
    }

    Q_FOREACH(const QString &strRuleName, objSettings.childGroups()){//for each rule
        objSettings.beginGroup(strRuleName);
        rulesList.append(new RuleObject(strRuleName,objSettings.getValue("enabled",false).toBool()));
        objSettings.endGroup();
    }

    objSettings.endGroup();//end rules

    ProfileClient *profileClient = new ProfileClient(NULL);

    view->rootContext()->setContextProperty("objProfileClient",profileClient);

    view->rootContext()->setContextProperty("objProximusLog",&objProximusLog);
    view->rootContext()->setContextProperty("objProximusUtils",&objproximusUtils);
    view->rootContext()->setContextProperty("objQSettings",&objSettings);
    view->rootContext()->setContextProperty("objRulesModel", QVariant::fromValue(rulesList));

    view->setSource(MDeclarativeCache::applicationDirPath()
                    + QLatin1String("/../qml/Proximus/main.qml"));

    QObject::connect(view->engine(), SIGNAL(quit()), view.data(), SLOT(close()));

    view->showFullScreen();

    app->exec();

}
