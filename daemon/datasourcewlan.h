#ifndef DATASOURCEWLAN_H
#define DATASOURCEWLAN_H

#include <QObject>
#include <QVector>
#include <QString>
#include "datasource.h"
#include "systemsnapshot.h"

class DataSourceWlan : public DataSource
{
    Q_OBJECT
public:
    explicit DataSourceWlan(SystemSnapshot *parent = 0);

signals:

public slots:
    void processSystemSnapshot();

private:
    int m_sourceRx;
    int m_sourceTx;

    QVector<int> m_prevBytesRx;
    QVector<int> m_prevBytesTx;
};

#endif // DATASOURCEWLAN_H
