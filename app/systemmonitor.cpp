#include "systemmonitor.h"
#include <QtDBus/QDBusConnection>

SystemMonitor::SystemMonitor(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Connect to dataUpdate event" <<
    QDBusConnection::sessionBus().connect(
        SYSMON_DBUS_SERVICE, SYSMON_DBUS_PATH, SYSMON_DBUS_IFACE,
        "dataUpdated", this, SIGNAL(dataUpdated())
    );
}

void SystemMonitor::clearData()
{
    m_storage.clearData();
    emit dataUpdated();
}

QVariant SystemMonitor::getData(DataSource::Type type, int depth, int width, bool avg)
{
    if (width == 0) {
        return QVariant();
    }
    QDateTime to = QDateTime::currentDateTimeUtc();
    QDateTime from = to.addSecs(-3600*depth);
    QVector<QVariantMap> data = m_storage.getData(type, from, to);
    return QVariant::fromValue(filterData(data, from, to, width, avg));
}

QList<QVariant> SystemMonitor::filterData(const QVector<QVariantMap>& data, const QDateTime &from, const QDateTime &to, int width, bool avg)
{
    QList<QVariant> points;
    qDebug() << "Filter Data" << data.size() << "to" << width;
    if (!data.size()) {
        return points;
    }

    qDebug() << "db minX" << data.front()["x"].toInt();
    qDebug() << "db maxX" << data.back()["x"].toInt();

    int minX = from.toTime_t(); //data.front()["x"].toInt();
    int maxX = to.toTime_t(); //data.back()["x"].toInt();
    int deltaX = (maxX - minX)/width;
    qDebug() << "minX" << minX << "maxX" << maxX << "deltaX" << deltaX;

    int pos = 0;
    double sumY = 0;
    int ptCount = 0;
    for(int dataX = minX; (dataX - deltaX) <= maxX && pos < data.size(); ) {
        const QVariantMap &point = data[pos];
        int pointX = point["x"].toInt();
        if (pointX > dataX) {
            double dataY = sumY;
            if (avg) {
                dataY /= ptCount;
            }
            while(pointX > dataX) {
                QVariantMap pt;
                pt["x"] = dataX;
                pt["y"] = dataY;
                points.append(pt);
                dataX += deltaX;
            }
            sumY = 0;
            ptCount = 0;
        }
        if (dataX >= pointX) {
            sumY += point["y"].toDouble();
            ptCount++;
            pos++;
        }
    }

    qDebug() << "Filtered Data" << points.size();
    qDebug() << "filtered minX" << points.front().toMap()["x"].toInt();
    qDebug() << "filtered maxX" << points.back().toMap()["x"].toInt();
    return points;
}

QVariant SystemMonitor::cpuTotal(int depth, int width)
{
    return getData(DataSource::CpuTotal, depth, width, true);
}

QVariant SystemMonitor::cpuUser(int depth, int width)
{
    return getData(DataSource::CpuUser, depth, width, true);
}

QVariant SystemMonitor::cpuSystem(int depth, int width)
{
    return getData(DataSource::CpuSystem, depth, width, true);
}

QVariant SystemMonitor::cpuIO(int depth, int width)
{
    return getData(DataSource::CpuIO, depth, width, true);
}

QVariant SystemMonitor::batteryCharge(int depth, int width)
{
    return getData(DataSource::BatteryPercentage, depth, width, true);
}

QVariant SystemMonitor::wlanRx(int depth, int width)
{
    return getData(DataSource::NetworkWlanRx, depth, width, false);
}

QVariant SystemMonitor::wlanTx(int depth, int width)
{
    return getData(DataSource::NetworkWlanTx, depth, width, false);
}


QVariant SystemMonitor::cellRx(int depth, int width)
{
    return getData(DataSource::NetworkCellRx, depth, width, false);
}

QVariant SystemMonitor::cellTx(int depth, int width)
{
    return getData(DataSource::NetworkCellTx, depth, width, false);
}
