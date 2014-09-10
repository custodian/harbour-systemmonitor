#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include "systemsnapshot.h"

class ApplicationInfo {
public:
    QString name;
    QString path;
    int id;
};

class DataSource:
    public QObject
{
    Q_OBJECT
    Q_ENUMS(DataType)
public:
    enum Type {
        CpuTotal = 100,
        CpuUser,
        CpuSystem,
        CpuIO,
        RAMUsed = 200,
        RAMCached,
        RAMBuffers,
        SwapUsed = 250,
        NetworkWlanTx = 300,
        NetworkWlanRx,
        NetworkCellTx,
        NetworkCellRx,
        BatteryPercentage = 400
    };

    DataSource(SystemSnapshot *parent = 0);

signals:
    void systemDataGathered(DataSource::Type type, float value);
    void applicationDataGathered(ApplicationInfo *appInfo, DataSource::Type type, float value);

protected:
    int registerSystemSource(const QString &source);
    int registerApplicationSource(const QString &source);

    const QByteArray & getSystemData(int source);
    //const QByteArray & getSystemData(const QString &source);

private:
    SystemSnapshot *m_snapshot;
};

#endif // DATASOURCE_H
