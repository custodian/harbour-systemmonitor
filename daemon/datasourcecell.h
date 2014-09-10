#ifndef DATASOURCECELL_H
#define DATASOURCECELL_H

#include <QObject>
#include <QVector>
#include <QString>
#include "datasource.h"
#include "systemsnapshot.h"

class DataSourceCell : public DataSource
{
    Q_OBJECT
public:
    explicit DataSourceCell(SystemSnapshot *parent = 0);

signals:

public slots:
    void processSystemSnapshot();

private:
    QVector<int> m_sourcesRx;
    QVector<int> m_sourcesTx;

    QVector<int> m_prevBytesRx;
    QVector<int> m_prevBytesTx;
};

#endif // DATASOURCECELL_H
