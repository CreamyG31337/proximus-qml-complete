#ifndef PROFILECLIENT_H
#define PROFILECLIENT_H

#if defined(Q_OS_SYMBIAN)

#elif defined(Q_WS_MAEMO_5)

#elif defined(Q_WS_SIMULATOR)

#else

#include <QString>
#include <QStringList>
#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>


//harmattan profile code provided by http://meegoharmattandev.blogspot.com/2011/07/changing-and-accessing-profiles.html
#define PROFILED_SERVICE "com.nokia.profiled"
#define PROFILED_PATH "/com/nokia/profiled"
#define PROFILED_INTERFACE "com.nokia.profiled"
#define PROFILED_GET_PROFILES "get_profiles"
#define PROFILED_SET_PROFILE "set_profile"
#define PROFILED_GET_VALUE "get_value"
#define PROFILED_PROFILE_CHANGED "profile_changed"
// The key for accessing Harmattan's profile type under profile
#define PROFILED_TYPE_VALUE "ringing.alert.type"

class ProfileClient : public QObject
{
    Q_OBJECT
public:
    ProfileClient(QObject*);
    bool setProfile(QString profileName);
    QStringList profileTypes() const;
Q_SIGNALS:
    void changed();
private:
    void profileChanged(bool changed, bool active, QString profile);
    QStringList profiles() const;
};

#endif
#endif // PROFILECLIENT_H
