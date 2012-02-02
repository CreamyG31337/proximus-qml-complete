#ifndef DBUSIFACE_H
#define DBUSIFACE_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusAbstractAdaptor>
#include <QPointer>
#include <controller.h>


class DbusIface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.appcheck.Proximus")
public:
    DbusIface(Controller *myController);

public slots:
   bool startTimer(int min, int max);

signals:
    void heartbeat();
    void didSomething(QString strInfo);

private:
    Controller *controller;
};

#endif // DBUSIFACE_H
