#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <QtSql>
#include "datasource.h"

class Storage : public QObject
{
    Q_OBJECT
public:
    explicit Storage(QObject *parent = 0);

    void clearData();
    QVector<QVariantMap> getSystemData(const QList<DataSource::Type> &types, const QDateTime &from, const QDateTime &to);
    //QVector<QVariantMap> getApplicationData();

    qint64 getDatabaseSize();
    qint64 getUnitsCollected();
signals:

public slots:
    void saveSystemData(const QDateTime &time, DataSource::Type type, float value);
    void removeObsoleteData(const QDateTime &time);

private:
    QString m_filePath;
    QSqlDatabase m_db;
};

#endif // STORAGE_H
