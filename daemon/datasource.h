#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

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
        MemoryTotal = 200,
        MemorySwap,
        NetworkWlanTx = 300,
        NetworkWlanRx,
        NetworkCellTx,
        NetworkCellRx,
        BatteryPercentage = 400,
        MemoryUsed = 500
    };

    DataSource(QObject *parent = 0);

    virtual void gatherData() = 0;

signals:
    void systemDataGathered(DataSource::Type type, float value);
    void applicationDataGathered(ApplicationInfo *appInfo, DataSource::Type type, float value);
};

#endif // DATASOURCE_H
