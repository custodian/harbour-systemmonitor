#ifndef DATASOURCEBATTERY_H
#define DATASOURCEBATTERY_H

#include <QObject>
#include <datasource.h>

class DataSourceBattery : public DataSource
{
    Q_OBJECT
public:
    explicit DataSourceBattery(QObject *parent = 0);

    virtual void gatherData();
signals:

public slots:

};

#endif // DATASOURCEBATTERY_H
