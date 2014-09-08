#include "datasourcecell.h"

#include <QDebug>
#include <QFile>

DataSourceCell::DataSourceCell(QObject *parent) :
    DataSource(parent)
{
    for (int i=0;i<=7;i++) {
        m_filesRx.append(QString("/sys/class/net/rmnet%1/statistics/rx_bytes").arg(i));
        m_filesTx.append(QString("/sys/class/net/rmnet%1/statistics/tx_bytes").arg(i));
    }
}

// /sys/class/net/rmnet[0-7]/statistics = rx_bytes | tx_bytes
void DataSourceCell::gatherData()
{
    qDebug() << "Network CELL data";

    int deltaRx = 0;
    int deltaTx = 0;

    QVector<int> bytesRx;
    QVector<int> bytesTx;
    foreach(const QString &filename, m_filesRx) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            bytesRx.append(QString(file.readLine()).toInt());
        } else {
            bytesRx.append(-1);
        }
    }

    foreach(const QString &filename, m_filesTx) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            bytesTx.append(QString(file.readLine()).toInt());
        } else {
            bytesTx.append(-1);
        }
    }

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

    emit dataGathered(DataSource::NetworkCellRx, deltaRx);
    emit dataGathered(DataSource::NetworkCellTx, deltaTx);
}
