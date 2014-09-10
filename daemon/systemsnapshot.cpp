#include "systemsnapshot.h"

#include <QFile>

SystemSnapshot::SystemSnapshot(QObject *parent) :
    QObject(parent)
{
    m_emptySource.reset(new QByteArray());
}

int SystemSnapshot::registerSystemSource(const QString &source)
{
    for (int i=0;i<m_systemSources.size();i++) {
        if (source == m_systemSources[i]) {
            return i;
        }
    }
    m_systemSources.append(source);
    return m_systemSources.size() - 1;
}

int SystemSnapshot::registerApplicationSource(const QString &source)
{
    for (int i=0;i<m_applicationSources.size();i++) {
        if (source == m_applicationSources[i]) {
            return i;
        }
    }
    m_applicationSources.append(source);
    return m_applicationSources.size() - 1;
}

const QByteArray& SystemSnapshot::getSystemData(int source)
{
    return *m_systemSnapshot.value(source, m_emptySource).data();
}

void SystemSnapshot::makeSnapshot()
{
    m_snapshotTime = QDateTime::currentDateTimeUtc();
    //system sources
    for (int i=0;i<m_systemSources.size();i++) {
        QFile source(m_systemSources[i]);
        if (source.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_systemSnapshot[i].reset(new QByteArray(source.readAll()));
        } else {
            m_systemSnapshot[i] = m_emptySource;
        }
    }

    //Get process pids

    //Get process data
}
