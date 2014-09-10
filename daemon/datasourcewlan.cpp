#include "datasourcewlan.h"
#include <QDebug>
#include <QFile>

DataSourceWlan::DataSourceWlan(SystemSnapshot *parent) :
    DataSource(parent)
{
    m_sourceRx = registerSystemSource("/sys/class/net/wlan0/statistics/rx_bytes");
    m_sourceTx = registerSystemSource("/sys/class/net/wlan0/statistics/tx_bytes");

    connect(parent, SIGNAL(processSystemSnapshot()), SLOT(processSystemSnapshot()));
}

// /sys/class/net/wlan0/statistics = rx_bytes | tx_bytes

void DataSourceWlan::processSystemSnapshot()
{
    qDebug() << "Network WLAN data";

    int deltaRx = 0;
    int deltaTx = 0;

    QVector<int> bytesRx;
    QVector<int> bytesTx;

    bytesRx.append(QString(getSystemData(m_sourceRx)).toInt());
    bytesTx.append(QString(getSystemData(m_sourceTx)).toInt());

    if (m_prevBytesRx.size() == bytesRx.size()) {
        for (int i=0;i<bytesRx.size();i++) {
            if (bytesRx[i] != -1 && m_prevBytesRx[i] != -1) {
                //Network was reseted
                if (bytesRx[i] < m_prevBytesRx[i]) {
                    deltaRx += bytesRx[i];
                } else {
                    deltaRx += (bytesRx[i] - m_prevBytesRx[i]);
                }
            }
        }
    }

    if (m_prevBytesTx.size() == bytesTx.size()) {
        for (int i=0;i<bytesTx.size();i++) {
            if (bytesTx[i] != -1 && m_prevBytesTx[i] != -1) {
                //Network was reseted
                if (bytesTx[i] < m_prevBytesTx[i]) {
                    deltaTx += bytesTx[i];
                } else {
                    deltaTx += (bytesTx[i] - m_prevBytesTx[i]);
                }
            }
        }
    }
    m_prevBytesRx = bytesRx;
    m_prevBytesTx = bytesTx;

    emit systemDataGathered(DataSource::NetworkWlanRx, deltaRx);
    emit systemDataGathered(DataSource::NetworkWlanTx, deltaTx);
}
