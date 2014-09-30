#include "datasourcebattery.h"

#include <QDebug>
#include <QFile>
#include <QString>

DataSourceBattery::DataSourceBattery(SystemSnapshot *parent) :
    DataSource(parent)
{
    m_batteryFull = registerSystemSource("/sys/class/power_supply/battery/charge_full");
    m_batteryNow = registerSystemSource("/sys/class/power_supply/battery/charge_now");
    m_capacity = registerSystemSource("/sys/class/power_supply/battery/capacity");

    connect(parent, SIGNAL(processSystemSnapshot()), SLOT(processSystemSnapshot()));
}

void DataSourceBattery::processSystemSnapshot()
{
    //qDebug() << "Battery DATA";
    const QByteArray &dataFull = getSystemData(m_batteryFull);
    const QByteArray &dataNow = getSystemData(m_batteryNow);
    const QByteArray &dataCap = getSystemData(m_capacity);
    float chargeLevel = -1;
    if (dataFull.size() && dataNow.size()) {
        int chargeFull = QString(dataFull).toInt();
        int chargeNow = QString(dataNow).toInt();

        chargeLevel = float(chargeNow) / chargeFull * 100;
    } else if (dataCap.size()){
        chargeLevel = QString(dataCap).toFloat();
    }
    if (chargeLevel < 0) {
        return;
    }
    emit systemDataGathered(DataSource::BatteryPercentage, chargeLevel);
}
