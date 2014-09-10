#ifndef DATASOURCEMEMORY_H
#define DATASOURCEMEMORY_H

#include "datasource.h"
#include "systemsnapshot.h"

class DataSourceMemory : public DataSource
{
    Q_OBJECT

public:
    explicit DataSourceMemory(SystemSnapshot *parent = 0);

signals:

public slots:
    void processSystemSnapshot();

private:
    int m_memInfo;
};

#endif // DATASOURCEMEMORY_H
