#ifndef DATASOURCECELL_H
#define DATASOURCECELL_H

#include <QObject>
#include <QVector>
#include <QString>
#include "datasource.h"

class DataSourceCell : public DataSource
{
    Q_OBJECT
public:
    explicit DataSourceCell(QObject *parent = 0);

    virtual void gatherData();
signals:

public slots:

private:
    QVector<QString> m_filesRx;
    QVector<QString> m_filesTx;

    QVector<int> m_prevBytesRx;
    QVector<int> m_prevBytesTx;
};

#endif // DATASOURCECELL_H
