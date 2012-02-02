#ifndef DBUSIFACE_H
#define DBUSIFACE_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusAbstractAdaptor>
#include <QSharedPointer>
#include <QApplication>

class DbusIface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.appcheck.Proximus.UI")
public:
    DbusIface(QObject *application);

public slots:
    void updateLog(QString text);
signals:
    void newLogInfo (const QVariant &logText);
private:

};

#endif // DBUSIFACE_H
