//this class was mostly stolen from here: http://www.developer.nokia.com/Community/Wiki/Create_calendar_events_with_alarms_with_Harmattan_platform_API
#ifndef CALWRAPPER_H
#define CALWRAPPER_H

#include <QObject>
#include <QSharedPointer>
#include <QLatin1String>
#include <QDateTime>

class CalWrapper : public QObject
{
    Q_OBJECT
public:
    CalWrapper(QObject *parent = 0);
    QString setAlarm(QDateTime start, QDateTime end, QString message);
    void removeAlarm(QString key, QDate date);
};

#endif // CALWRAPPER_H
