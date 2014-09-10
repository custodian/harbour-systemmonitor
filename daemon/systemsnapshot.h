#ifndef SYSTEMSNAPSHOT_H
#define SYSTEMSNAPSHOT_H

#include <QObject>
#include <QDateTime>
#include <QHash>
#include <QVector>
#include <QSharedPointer>

class SystemSnapshot : public QObject
{
    Q_OBJECT
public:
    typedef QHash<int, QSharedPointer<QByteArray>> Snapshot;

    explicit SystemSnapshot(QObject *parent = 0);

    int registerSystemSource(const QString &source);
    int registerApplicationSource(const QString &source);

    const QByteArray& getSystemData(int source);
    //void getApplicationSnapshot();

signals:
    void processSystemSnapshot();
    void processApplicationSnapshot();

public slots:

protected:
    void makeSnapshot();
    QDateTime m_snapshotTime;

private:
    QVector<QString> m_systemSources;
    QVector<QString> m_applicationSources;

    Snapshot m_systemSnapshot;
    QList<QSharedPointer<Snapshot> > m_applicationSnapshot;
    QSharedPointer<QByteArray> m_emptySource;
};

#endif // SYSTEMSNAPSHOT_H
