#ifndef DATASOURCEBATTERY_H
#define DATASOURCEBATTERY_H

#include <QObject>
#include <datasource.h>
#include "systemsnapshot.h"

class DataSourceBattery : public DataSource
{
    Q_OBJECT

public:
    explicit DataSourceBattery(SystemSnapshot *parent = 0);

signals:

public slots:
    void processSystemSnapshot();

private:
    int m_batteryFull;
    int m_batteryNow;
};

#endif // DATASOURCEBATTERY_H
