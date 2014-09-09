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

    QVector<int> current;
    //skip cpu, use cpu0, cpu1, ...
    //because cpu1 resets idle jiffs during low power mode
    QString cpu = stat.readLine();
    cpu = stat.readLine();
    while(cpu.indexOf("cpu") == 0) {
        QStringList jiffs = cpu.split(" ",QString::SkipEmptyParts);
        jiffs.pop_front();

        //user nice system idle io irq ...
        if (jiffs.size() < JIFF_COUNT) {
            qDebug() << "Error fetching CPU jiffs";
        }
        for (int i=0; i<JIFF_COUNT; i++) {
            current.append(jiffs[i].toInt());
        }

        cpu = stat.readLine();
    }

    if (m_prev.size() == current.size()) {
        int sum = 0;
        QVector<int> delta;
        delta.fill(0, JIFF_COUNT);
        for (int i=0; i<current.size(); i++) {
            int diff = current[i]-m_prev[i];
            if (diff < 0) {
                qDebug() << "ATTENTION! Mismatch" << i << m_prev[i] << ">" << current[i];
                diff = current[i];
            }
            sum += diff;
            delta[i%JIFF_COUNT] += diff;
        }

        emit systemDataGathered(DataSource::CpuTotal, float(sum - (delta[3]+delta[4])) / sum * 100);
        emit systemDataGathered(DataSource::CpuUser, float(delta[0]+delta[1]) / sum * 100);
        emit systemDataGathered(DataSource::CpuSystem, float(delta[2]) / sum * 100);
        emit systemDataGathered(DataSource::CpuIO, float(delta[4]) / sum * 100);
    }
    m_prev = current;
}
