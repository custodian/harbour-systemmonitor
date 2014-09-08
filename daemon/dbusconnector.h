#ifndef DBUSCONNECTOR_H
#define DBUSCONNECTOR_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

class DBusConnector : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList services READ services NOTIFY servicesChanged)

    QStringList dbusServices;

public:
    explicit DBusConnector(QObject *parent = 0);

    QStringList services() { return dbusServices; }

signals:
    void servicesChanged();

public slots:

protected slots:
    void onServiceRegistered(const QString &);
    void onServiceUnregistered(const QString &);

};

#endif // DBUSCONNECTOR_H
