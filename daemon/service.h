#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QTimer>
#include <QtSql>

#include "settings.h"
#include "dbusadapter.h"
#include "storage.h"
#include "datasource.h"

#include "keepalive/backgroundactivity.h"

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(QObject *parent, Settings *settings);

signals:
    void dataUpdated();

public slots:
    void dataGathered(DataSource::Type type, float value);
    void updateIntervalChanged(int interval);

private slots:
    void initDataSources();
    void backgroundRunning();
    void gatherData();
    void removeObsoleteData();

private:
    DBusAdapter *dbus;
    Settings *m_settings;
    QList<DataSource*> m_sources;

    //TODO: change to BackgroundActivity from 'libkeepalive-devel'
    BackgroundActivity *m_background;
    //QTimer m_timer;
    QDateTime m_updateTime;

    Storage m_storage;
};

#endif // SERVICE_H
