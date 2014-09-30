#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QTimer>
#include <QtSql>

#include "settings.h"
#include "dbusadapter.h"
#include "storage.h"
#include "datasource.h"
#include "systemsnapshot.h"

#include "keepalive/backgroundactivity.h"

typedef QList<QPair<DataSource::Type, float> > GatheredData;

class Service : public SystemSnapshot
{
    Q_OBJECT
public:
    explicit Service(QObject *parent, Settings *settings);

signals:
    void dataUpdated();

public slots:
    void systemDataGathered(DataSource::Type type, float value);
    void applicationDataGathered(int appid, DataSource::Type type, float value);
    void updateIntervalChanged(int interval);

private slots:
    void initDataSources();
    void backgroundRunning();
    void gatherData();
    void removeObsoleteData();
    void commitGatheredData();

private:
    DBusAdapter *dbus;
    Settings *m_settings;
    QList<DataSource*> m_sources;

    GatheredData m_gatheredSystemData;
    QHash<int, GatheredData> m_gatheredAppData;

    BackgroundActivity *m_background;
    Storage m_storage;
};

#endif // SERVICE_H
