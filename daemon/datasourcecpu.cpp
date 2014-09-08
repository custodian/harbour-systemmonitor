#include "datasourcecpu.h"
#include "storage.h"

DataSourceCPU::DataSourceCPU(QObject *parent):
    DataSource(parent)
{
}

void DataSourceCPU::gatherData()
{
    qDebug() << "CPU DATA";
    QFile stat("/proc/stat");
    if (!stat.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening stat file";
        return;
    }

    //TODO: skip cpu, use cpu0, cpu1
    QString cpu = stat.readLine();
    QStringList jiffs = cpu.split(" ",QString::SkipEmptyParts);
    jiffs.pop_front();

    //user nice system idle io irq ...
    QVector<int> current;
    if (jiffs.size() <5) {
        qDebug() << "Error fetching CPU jiffs";
    }
    for (int i=0;i<5;i++) {
        current.append(jiffs[i].toInt());
    }

    if (m_prev.size() == current.size()) {
        int sum = 0;
        QVector<int> delta;
        for (int i=0;i<5/*current.size()*/;i++) {
            int diff = current[i]-m_prev[i];
            if (diff < 0) {
                qDebug() << "ATTENTION! Mismatch" << m_prev << current;
                diff = current[i];
            }
            sum += diff;
            delta.append(diff);
        }

        emit dataGathered(DataSource::CpuTotal, float(sum - (delta[3]+delta[4])) / sum * 100);
        emit dataGathered(DataSource::CpuUser, float(delta[0]+delta[1]) / sum * 100);
        emit dataGathered(DataSource::CpuSystem, float(delta[2]) / sum * 100);
        emit dataGathered(DataSource::CpuIO, float(delta[4]) / sum * 100);
    }
    m_prev = current;
}
