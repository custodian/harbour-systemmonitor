#include "datasourcebattery.h"

#include <QDebug>
#include <QFile>
#include <QString>

DataSourceBattery::DataSourceBattery(QObject *parent) :
    DataSource(parent)
{
}

void DataSourceBattery::gatherData()
{
    qDebug() << "Battery DATA";
    QFile fileFull("/sys/class/power_supply/battery/charge_full");
    if (!fileFull.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening charge_full file";
        return;
    }

    QFile fileNow("/sys/class/power_supply/battery/charge_now");
    if (!fileNow.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening charge_now file";
        return;
    }

    int chargeFull = QString(fileFull.readLine()).toInt();
    int chargeNow = QString(fileNow.readLine()).toInt();

    emit systemDataGathered(DataSource::BatteryPercentage, float(chargeNow) / chargeFull * 100);
}
