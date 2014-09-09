#ifndef DATASOURCECPU_H
#define DATASOURCECPU_H

#include <QObject>
#include <QVector>
#include "datasource.h"

class DataSourceCPU:
    public DataSource
{
    Q_OBJECT

    //jiffs count readed from /proc/stat line
    static const int JIFF_COUNT = 5;

public:
    DataSourceCPU(QObject *parent = 0);

    virtual void gatherData();

private:
    QVector<int> m_prev;
};

#endif // DATASOURCECPU_H
