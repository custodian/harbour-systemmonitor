#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QObject>
#include <QDBusInterface>
#include "../daemon/storage.h"
#include "../daemon/settings.h"

#define SYSMON_SYSTEMD_UNIT "harbour-systemmonitord.service"
#define SYSMON_DBUS_SERVICE "net.thecust.systemmonitor"
#define SYSMON_DBUS_PATH    "/net/thecust/systemmonitor"
#define SYSMON_DBUS_IFACE   "net.thecust.systemmonitor"

class SystemMonitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autorun READ autorun WRITE setAutorun NOTIFY autorunChanged)
    bool autorun() const;
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    bool enabled() const;

public:
    explicit SystemMonitor(QObject *parent = 0);

signals:
    void dataUpdated();

    void enabledChanged();
    void autorunChanged();

public slots:
    void updateIntervalChanged(int interval);

    void setEnabled(bool);
    void setAutorun(bool);

    void clearData();
    QVariant getDatabaseSize();
    QVariant getUnitsCollected();

    QVariant getSystemGraph(QVariantList types, int depth, int width, bool avg);

private slots:
    void getUnitProperties();
    void onPropertiesChanged(QString interface, QMap<QString, QVariant> changed, QStringList invalidated);

private:
    QVariant getSystemData(DataSource::Type type, int depth, int width, bool avg);
    QVariant getSystemData(const QList<DataSource::Type> &types, int depth, int width, bool avg);

    QList<QVariant> filterData(const QVector<QVariantMap>& data, const QDateTime &from, const QDateTime &to, int width, bool avg);

    QDBusInterface *systemd;
    QDBusObjectPath unitPath;
    QVariantMap unitProperties;

    QTimer m_updateTimer;
    Storage m_storage;
    Settings m_settings;
};

#endif // SYSTEMMONITOR_H
