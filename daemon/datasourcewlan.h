#ifndef DATASOURCEWLAN_H
#define DATASOURCEWLAN_H

#include <QObject>
#include <QVector>
#include <QString>
#include "datasource.h"

class DataSourceWlan : public DataSource
{
    Q_OBJECT
public:
    explicit DataSourceWlan(QObject *parent = 0);

    virtual void gatherData();
signals:

public slots:

private:
    QVector<QString> m_filesRx;
    QVector<QString> m_filesTx;

    QVector<int> m_prevBytesRx;
    QVector<int> m_prevBytesTx;
};

#endif // DATASOURCEWLAN_H
