#include "systemsnapshot.h"

#include <QDebug>
#include <QFile>
#include <QDir>

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

    //TODO: Get process pids
    /*
    QDir proc("/proc");
    QStringList pids = proc.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    qDebug () << "PIDS" << pids;

    QRegExp numbers("[0-9]+");
    foreach(const QString &pid, pids) {
        if (!numbers.exactMatch(pid)) {
            continue;
        }

        if (!proc.cd(pid)) continue;
        //get default process data
        qDebug() << "pid" << pid;


        //get registered data

        proc.cdUp();
    }
    */
}
