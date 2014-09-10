#include "storage.h"

#include <QStandardPaths>

Storage::Storage(QObject *parent) :
    QObject(parent)
{
    QDir dataPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    if (!dataPath.exists()) {
        dataPath.mkpath(".");
    }
    m_filePath = dataPath.absoluteFilePath("base.sqlite");
    qDebug() << "Opening database" << m_filePath;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_filePath);
    m_db.open();
    if (m_db.isOpen()) {
        m_db.transaction();
        if (!m_db.tables().contains("data")) {
            m_db.exec("CREATE TABLE data ( "
                          "time INTEGER NOT NULL, "
                          "type INTEGER NOT NULL, "
                          "value REAL NOT NULL,  "
                          "PRIMARY KEY(time ASC, type ASC) "
            ")");
        }
        m_db.commit();
    } else {
        qDebug() << "Failed to open database";
    }
}

qint64 Storage::getDatabaseSize()
{
    return QFileInfo(m_filePath).size();
}

qint64 Storage::getUnitsCollected()
{
    qint64 count = 0;
    QSqlQuery query(m_db);
    if (query.exec("SELECT count(*) as cnt FROM data")) {
        if (query.next()) {
            count = query.value("cnt").toInt();
        }
    }
    return count;
}

void Storage::clearData()
{
    QSqlQuery query(m_db);
    query.exec("DELETE FROM data");
}

QVector<QVariantMap> Storage::getSystemData(const QList<DataSource::Type> &types, const QDateTime &from, const QDateTime &to)
{
    //qDebug() << "getData" << type << from << to;
    QStringList strTypes;
    foreach(DataSource::Type type, types) {
        strTypes.append(QString::number(int(type)));
    }
    QVector<QVariantMap> data;
    QSqlQuery query(m_db);
    query.prepare(QString("SELECT time, SUM(value) as value"
                  " FROM data "
                  " WHERE "
                  " type IN (%1) "
                  " AND time BETWEEN :from AND :to "
                  " GROUP BY time "
                  " ORDER BY time ASC").arg(strTypes.join(',')));
    //QSqlQuery cannot bind string into value;
    //query.bindValue(":type", strTypes.join(','));
    query.bindValue(":from", from.toTime_t());
    query.bindValue(":to", to.toTime_t());
    if (query.exec()) {
        while(query.next()) {
            QVariantMap pt;
            pt["x"] = query.value("time");
            pt["y"] = query.value("value");
            data.append(pt);
        }
    } else {
        qDebug() << "getSystemData error:" << query.lastError().text();
    }
    return data;
}

void Storage::removeObsoleteData(const QDateTime &time)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM data WHERE time < :time");
    query.bindValue(":time", time.toTime_t());
    //qDebug() << "Remove obsolete data" <<
    query.exec();
}

void Storage::saveSystemData(const QDateTime &time, DataSource::Type type, float value)
{
    //TODO: use execBatch & transaction ??
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO data(time, type, value) VALUES(:time, :type, :value)");
    query.bindValue(":time", time.toTime_t());
    query.bindValue(":type", type);
    query.bindValue(":value", value);
    //qDebug () << "Saving data" <<
    query.exec();
}
