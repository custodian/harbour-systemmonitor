#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QObject>
#include "../daemon/storage.h"

#define SYSMON_DBUS_SERVICE "net.thecust.systemmonitor"
#define SYSMON_DBUS_PATH    "/net/thecust/systemmonitor"
#define SYSMON_DBUS_IFACE   "net.thecust.systemmonitor"

class SystemMonitor : public QObject
{
    Q_OBJECT
public:
    explicit SystemMonitor(QObject *parent = 0);

signals:
    void dataUpdated();

public slots:
    void clearData();

    QVariant cpuTotal(int depth, int width);
    QVariant cpuUser(int depth, int width);
    QVariant cpuSystem(int depth, int width);
    QVariant cpuIO(int depth, int width);

    QVariant batteryCharge(int depth, int width);

    QVariant wlanRx(int depth, int width);
    QVariant wlanTx(int depth, int width);

    QVariant cellRx(int depth, int width);
    QVariant cellTx(int depth, int width);

private:
    QVariant getData(DataSource::Type type, int depth, int width, bool avg);
    QList<QVariant> filterData(const QVector<QVariantMap>& data, const QDateTime &from, const QDateTime &to, int width, bool avg);

    Storage m_storage;
};

#endif // SYSTEMMONITOR_H
