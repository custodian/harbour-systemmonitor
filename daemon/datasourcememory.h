#ifndef DATASOURCEMEMORY_H
#define DATASOURCEMEMORY_H

#include "datasource.h"

class DataSourceMemory : public DataSource
{
    Q_OBJECT
public:
    explicit DataSourceMemory(QObject *parent = 0);

    virtual void gatherData();
signals:

public slots:

};

#endif // DATASOURCEMEMORY_H
