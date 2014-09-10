#include "datasourcecell.h"

#include <QDebug>

DataSourceCell::DataSourceCell(SystemSnapshot *parent) :
    DataSource(parent)
{
    for (int i=0;i<=7;i++) {
        m_sourcesRx.append(registerSystemSource(QString("/sys/class/net/rmnet%1/statistics/rx_bytes").arg(i)));
        m_sourcesTx.append(registerSystemSource(QString("/sys/class/net/rmnet%1/statistics/tx_bytes").arg(i)));
    }

    connect(parent, SIGNAL(processSystemSnapshot()), SLOT(processSystemSnapshot()));
}

// /sys/class/net/rmnet[0-7]/statistics = rx_bytes | tx_bytes
void DataSourceCell::processSystemSnapshot()
{
    //qDebug() << "Network CELL data";
    int deltaRx = 0;
    int deltaTx = 0;

    QVector<int> bytesRx;
    QVector<int> bytesTx;
    foreach(int sourceRx, m_sourcesRx) {
        bytesRx.append(QString(getSystemData(sourceRx)).toInt());
    }

    foreach(int sourceTx, m_sourcesTx) {
        bytesTx.append(QString(getSystemData(sourceTx)).toInt());
    }

    if (m_prevBytesRx.size() == bytesRx.size()) {
        for (int i=0;i<bytesRx.size();i++) {
            //Network was reseted
            if (bytesRx[i] < m_prevBytesRx[i]) {
                deltaRx += bytesRx[i];
            } else {
                deltaRx += (bytesRx[i] - m_prevBytesRx[i]);
            }
        }
    }

    if (m_prevBytesTx.size() == bytesTx.size()) {
        for (int i=0;i<bytesTx.size();i++) {
            if (bytesTx[i] < m_prevBytesTx[i]) {
                deltaTx += bytesTx[i];
            } else {
                deltaTx += (bytesTx[i] - m_prevBytesTx[i]);
            }
        }
    }
    m_prevBytesRx = bytesRx;
    m_prevBytesTx = bytesTx;

    emit systemDataGathered(DataSource::NetworkCellRx, deltaRx);
    emit systemDataGathered(DataSource::NetworkCellTx, deltaTx);
}
