#include "storage.h"

#include <QStandardPaths>

Storage::Storage(QObject *parent) :
    QObject(parent)
{
    QDir dataPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    if (!dataPath.exists()) {
        dataPath.mkpath(".");
    }
    QString filePath = dataPath.absoluteFilePath("base.sqlite");
    qDebug() << "Opening database" << filePath;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(filePath);
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

void Storage::clearData()
{
    QSqlQuery query(m_db);
    query.exec("DELETE FROM data");
}

QVector<QVariantMap> Storage::getData(DataSource::Type type, const QDateTime &from, const QDateTime &to) {
    qDebug() << "getData" << type << from << to;
    QVector<QVariantMap> data;
    QSqlQuery query(m_db);
    query.prepare("SELECT time, value FROM data WHERE type = :type AND time BETWEEN :from AND :to ORDER BY time ASC");
    query.bindValue(":type", type);
    query.bindValue(":from", from.toTime_t());
    query.bindValue(":to", to.toTime_t());
    query.exec();
    while(query.next()) {
        QVariantMap pt;
        pt["x"] = query.value("time");
        pt["y"] = query.value("value");
        data.append(pt);
    }
    return data;
}

void Storage::removeObsoleteData(const QDateTime &time) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM data WHERE time < :time");
    query.bindValue(":time", time.toTime_t());
    qDebug() << "Remove obsolete data" << query.exec();
}

void Storage::save(DataSource::Type type, const QDateTime &time, float value) {

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO data(time, type, value) VALUES(:time, :type, :value)");
    query.bindValue(":time", time.toTime_t());
    query.bindValue(":type", type);
    query.bindValue(":value", value);
    qDebug () << "Saving data" << query.exec();
}
