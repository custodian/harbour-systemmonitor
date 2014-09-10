#ifndef DATASOURCECPU_H
#define DATASOURCECPU_H

#include <QObject>
#include <QVector>
#include "datasource.h"
#include "systemsnapshot.h"

class DataSourceCPU:
    public DataSource
{
    Q_OBJECT

    //jiffs count readed from /proc/stat line
    static const int JIFF_COUNT = 5;

public:
    DataSourceCPU(SystemSnapshot *parent = 0);

public slots:
    void processSystemSnapshot();

private:
    int m_cpuStat;
    QVector<int> m_prev;
};

#endif // DATASOURCECPU_H
