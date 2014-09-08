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
    QVector<QVariantMap> getData(DataSource::Type type, const QDateTime &from, const QDateTime &to);

signals:

public slots:
    void save(DataSource::Type type, const QDateTime &time, float value);
    void removeObsoleteData(const QDateTime &time);

private:
    QSqlDatabase m_db;
};

#endif // STORAGE_H
