#include "profileclient.h"

#if !defined (Q_WS_SIMULATOR)

ProfileClient::ProfileClient(QObject *parent) : QObject(parent)
{
   QDBusConnection::sessionBus().connect("", "",
       PROFILED_INTERFACE,
       PROFILED_PROFILE_CHANGED,
       this,
       SLOT(profileChanged(bool, bool, QString))
       );
}

void ProfileClient::profileChanged(bool isChanged, bool active, QString profile)
{
    if (isChanged && active) {
        Q_EMIT changed();
        qDebug("Profile changed to %s", qPrintable(profile));
    }
}

QStringList ProfileClient::profiles() const
{
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    QDBusReply<QStringList> reply =
        dbus_iface.call(PROFILED_GET_PROFILES);
    return reply.value();
}

QStringList ProfileClient::profileTypes() const
{
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    QStringList profileTypes;
    QString profile;
    foreach (profile, profiles())
    {
        QDBusReply<QString> reply =
            dbus_iface.call(PROFILED_GET_VALUE, profile,
                            PROFILED_TYPE_VALUE);
        profileTypes.append(reply.value());
    }

    // In Harmattan at least, profile type Ringing is
    // attached to  "general" and "outdoors" profiles.
    // The "general" profile is the one that's used for
    // "ringing", profile "outdoors" should not be used
    // when setting a profile.
    profileTypes.removeDuplicates();
    return profileTypes;
}

bool ProfileClient::setProfile(QString profileName)
{
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    if (profileName == "Silent") profileName = "silent";
    if (profileName == "Beep") profileName = "meeting";
    if (profileName == "Ringing") profileName = "general";

    // Returns true if success
    QDBusReply<bool> reply =
        dbus_iface.call(PROFILED_SET_PROFILE, profileName);
    return reply.value();
}
#endif
