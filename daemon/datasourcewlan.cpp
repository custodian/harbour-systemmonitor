#include "datasourcewlan.h"
#include <QDebug>
#include <QFile>

DataSourceWlan::DataSourceWlan(QObject *parent) :
    DataSource(parent)
{
    m_filesRx.append("/sys/class/net/wlan0/statistics/rx_bytes");
    m_filesTx.append("/sys/class/net/wlan0/statistics/tx_bytes");
}

// /sys/class/net/wlan0/statistics = rx_bytes | tx_bytes

void DataSourceWlan::gatherData()
{
    qDebug() << "Network WLAN data";

    int deltaRx = 0;
    int deltaTx = 0;

    QVector<int> bytesRx;
    QVector<int> bytesTx;
    foreach(const QString &filename, m_filesRx) {
        QFile fileRx(filename);
        if (fileRx.open(QIODevice::ReadOnly | QIODevice::Text)) {
            bytesRx.append(QString(fileRx.readLine()).toInt());
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

    emit systemDataGathered(DataSource::NetworkWlanRx, deltaRx);
    emit systemDataGathered(DataSource::NetworkWlanTx, deltaTx);
}
