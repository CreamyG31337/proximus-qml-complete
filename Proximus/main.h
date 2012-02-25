#ifndef MAIN_H
#define MAIN_H

#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative>
#include <MDeclarativeCache>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QtCore/QCoreApplication>
#include <QtCore/QtGlobal>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QAbstractListModel>
#include <QtCore/QScopedPointer>
#include <QProcess>
#include <QDebug>
#include <QSharedPointer>
#include "qobjectlistmodel.h"
#include <QtAlgorithms>
#include <QStack>
#include <QMap>


class ProximusLog : public QObject
{
    Q_OBJECT

signals:
    void newLogInfo(const QVariant &LogText);
};


class ProximusUtils : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE QString isServiceRunning();
    Q_INVOKABLE void refreshRulesModel();
    Q_INVOKABLE void moveRuleUp(int rulenum);
    Q_INVOKABLE void moveRuleDown(int rulenum);
    Q_INVOKABLE void deleteRule(int rulenum);
    //QList<QObject*> *rules_ptr;
    QObjectListModel *myModel;
    QSharedPointer<QDeclarativeView> view_ptr;
    QMap<int,QString> RuleMap;
};

//annoying wrapper class for qsettings
class MySettings : public QObject
{
    Q_OBJECT
public:
    explicit MySettings();
    virtual ~MySettings();
    Q_INVOKABLE QVariant getValue(QString keyval,QVariant defaultval){
        return qsettInternal->value(keyval,defaultval);
    }
    Q_INVOKABLE void setValue(QString key,QVariant value){
        qsettInternal->setValue(key,value);
    }
    Q_INVOKABLE void beginGroup(QString prefix){
        qsettInternal->beginGroup(prefix);
    }
    Q_INVOKABLE void endGroup(){
        qsettInternal->endGroup();
    }
    Q_INVOKABLE bool contains(QString key){
        return qsettInternal->contains(key);
    }
    Q_INVOKABLE QStringList childGroups(){
        return qsettInternal->childGroups();
    }
    Q_INVOKABLE QString group(){
        return qsettInternal->group();
    }
    Q_INVOKABLE void remove(QString key){
        qsettInternal->remove(key);
    }
    Q_INVOKABLE QString fileName(){
        return qsettInternal->fileName();
    }
    Q_INVOKABLE void clear(){
        qsettInternal->clear();
    }
private:
    QSettings *qsettInternal;
};

//single rule
class RuleObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY myModelChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY myModelChanged)
    Q_PROPERTY(int number READ number WRITE setNumber NOTIFY myModelChanged)
public:
    RuleObject(QString name, bool enabled, int number);
    ~RuleObject();
    void setEnabled(bool enabled);
    void setName(QString name);
    void setNumber(int number);
    bool enabled();
    QString name();
    int number();
signals:
    void myModelChanged();
private:
    QString strname;
    bool boolenabled;
    int ruleNumber;
};
#endif // MAIN_H
