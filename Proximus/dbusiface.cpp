#include "dbusiface.h"
#include <QDebug>
#include <QObject>

DbusIface::DbusIface(QObject* application)
    : QDBusAbstractAdaptor(application)
{
    QObject::connect(this,SIGNAL (newLogInfo(QVariant)),
           application,SIGNAL (newLogInfo(QVariant)));
}

void DbusIface::updateLog(QString text)
{
    emit newLogInfo(text);
}
