#include "datasourcebattery.h"

#include <QDebug>
#include <QFile>
#include <QString>

DataSourceBattery::DataSourceBattery(SystemSnapshot *parent) :
    DataSource(parent)
{
    m_batteryFull = registerSystemSource("/sys/class/power_supply/battery/charge_full");
    m_batteryNow = registerSystemSource("/sys/class/power_supply/battery/charge_now");

    connect(parent, SIGNAL(processSystemSnapshot()), SLOT(processSystemSnapshot()));
}

void DataSourceBattery::processSystemSnapshot()
{
    //qDebug() << "Battery DATA";
    const QByteArray &dataFull = getSystemData(m_batteryFull);
    const QByteArray &dataNow = getSystemData(m_batteryNow);
    if (!dataFull.size() || !dataNow.size()) {
        return;
    }
    int chargeFull = QString(dataFull).toInt();
    int chargeNow = QString(dataNow).toInt();

    emit systemDataGathered(DataSource::BatteryPercentage, float(chargeNow) / chargeFull * 100);
}
