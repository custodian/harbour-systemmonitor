#include "datasourcebattery.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>

DataSourceBattery::DataSourceBattery(SystemSnapshot *parent) :
    DataSource(parent)
{
    QDir battery("/sys/class/power_supply");
    QStringList filters;
    filters << "*battery";
    battery.setNameFilters(filters);

    const QStringList files = battery.entryList();
    QStringListIterator iterator(files);
    QString fileName;
    if ( iterator.hasNext() ) {
        fileName = battery.absoluteFilePath(iterator.next());
        qDebug() << "Found battery root: " << fileName;
    }
    m_batteryFull = registerSystemSource(fileName + "/charge_full");
    m_batteryNow = registerSystemSource(fileName + "/charge_now");
    m_capacity = registerSystemSource(fileName + "/capacity");

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
