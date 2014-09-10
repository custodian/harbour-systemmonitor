#include "service.h"
#include "datasourcecpu.h"
#include "datasourcebattery.h"
#include "datasourcecell.h"
#include "datasourcewlan.h"
#include "datasourcememory.h"

Service::Service(QObject *parent, Settings *settings) :
    SystemSnapshot(parent), m_settings(settings)
{
    initDataSources();

    connect(m_settings, SIGNAL(updateIntervalChanged(int)), SLOT(updateIntervalChanged(int)));

    m_background = new BackgroundActivity(this);
    connect(m_background, SIGNAL(running()), SLOT(backgroundRunning()));
    m_background->setWakeupFrequency(BackgroundActivity::Range);

    //TODO: dbus disabled since until deepsleep mode investigated
    //dbus = new DBusAdapter(this);

    updateIntervalChanged(m_settings->updateInterval);
}

void Service::initDataSources()
{
    m_sources.append(new DataSourceCPU(this));
    m_sources.append(new DataSourceBattery(this));
    m_sources.append(new DataSourceWlan(this));
    m_sources.append(new DataSourceCell(this));
    m_sources.append(new DataSourceMemory(this));
    //m_sources.append(new DataSourceTemp(this));

    foreach(const DataSource* source, m_sources) {
        connect(source, SIGNAL(systemDataGathered(DataSource::Type,float))
                , SLOT(systemDataGathered(DataSource::Type,float)));
        connect(source, SIGNAL(applicationDataGathered(ApplicationInfo*,DataSource::Type,float))
                , SLOT(applicationDataGathered(ApplicationInfo*,DataSource::Type,float)));
    }
}

void Service::backgroundRunning()
{
    gatherData();
    m_background->wait();
}

void Service::updateIntervalChanged(int interval)
{
    //qDebug() << "Update interval" << interval;
    m_background->setWakeupRange(interval, interval);
    if (m_background->state() == BackgroundActivity::Stopped) {
        m_background->run();
    }
}

void Service::gatherData()
{
    //qDebug() << "Gather data start";
    makeSnapshot();

    emit processSystemSnapshot();
    emit processApplicationSnapshot();

    commitGatheredData();
    removeObsoleteData();

    //TODO: dont emit when deep sleep mode and/or ignore dbus
    //Disabled until investigated
    //emit dataUpdated();
}

void Service::commitGatheredData()
{
    auto it = m_gatheredSystemData.begin();
    while(it != m_gatheredSystemData.end()) {
        m_storage.saveSystemData(m_snapshotTime, it->first, it->second);
        it++;
    }
    m_gatheredSystemData.clear();
}

void Service::removeObsoleteData() {
    m_storage.removeObsoleteData(m_snapshotTime.addDays(-1 * m_settings->updateInterval));
}


void Service::systemDataGathered(DataSource::Type type, float value) {
    //qDebug() << "DataGathered" << type << value;
    m_gatheredSystemData.append(qMakePair(type, value));
}

void Service::applicationDataGathered(ApplicationInfo *appInfo, DataSource::Type type, float value)
{
    //TODO: app detection via kernel module or netlink connector
    //http://stackoverflow.com/questions/6075013/linux-detect-launching-of-programs
    //TODO: m_storage.saveApplicationData(appInfo, type, value);
}
