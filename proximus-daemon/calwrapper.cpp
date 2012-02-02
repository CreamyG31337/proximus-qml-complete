#include "mkcal/extendedcalendar.h"
#include "mkcal/extendedstorage.h"
#include "mkcal/notebook.h"
#include "kcalcoren/event.h"
#include "kcalcoren/alarm.h"
#include "kcalcoren/kdatetime.h"
#include "kcalcoren/duration.h"
#include "calwrapper.h"
#include <QtCore/QDebug>

using namespace KCalCore;
using namespace mKCal;

CalWrapper::CalWrapper(QObject *parent) : QObject(parent)
{
}

QString CalWrapper::setAlarm(QDateTime start, QDateTime end, QString message)
{
    ExtendedCalendar::Ptr calendar = ExtendedCalendar::Ptr ( new ExtendedCalendar( KDateTime::Spec::LocalZone() ) );
    ExtendedStorage::Ptr storage = calendar->defaultStorage(calendar);
    Event::Ptr event;
    Alarm::Ptr alarm;

    storage->open();
    storage->load(start.date(), end.date());

    event=Event::Ptr(new Event());
    event->setDtStart(KDateTime(start));
    event->setDescription(message);
    event->setSummary(message);

    alarm=Alarm::Ptr(new Alarm(event.data()));
    alarm->setDisplayAlarm(message);
    alarm->setEnabled(true);
    alarm->setRepeatCount(1);
    alarm->setStartOffset(Duration(0));
    alarm->setTime(KDateTime(start));
    alarm->setEndOffset(Duration(0));

    event->addAlarm(alarm);

    // the second parameter refers to the personal calendar uid
    calendar->addEvent(event, "11111111-2222-3333-4444-555555555555");
    calendar->save();

    storage->save();
    storage->close();

    qDebug() << "CalWrapper::setAlarm:Alarm set";
    return event->uid();
}

void CalWrapper::removeAlarm(QString key, QDate date)
{
    ExtendedCalendar::Ptr calendar = ExtendedCalendar::Ptr ( new ExtendedCalendar( KDateTime::Spec::LocalZone() ) );
    ExtendedStorage::Ptr storage = calendar->defaultStorage(calendar);
    Event::Ptr event;

    storage->open();
    storage->load(date.addDays(-15), date.addDays(15));

    event=calendar->event(key);
    if (event.data()==NULL)
        qDebug() << "CalWrapper::removeAlarm:Event doesnt exist";
    else
    {
        calendar->deleteEvent(event);
        calendar->save();
        storage->save();

        qDebug() << "CalWrapper::Alarm removed";
    }
    storage->close();
}


