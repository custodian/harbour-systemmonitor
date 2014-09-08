#include "dbusconnector.h"

#include <QDebug>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusConnectionInterface>

DBusConnector::DBusConnector(QObject *parent) :
    QObject(parent)
{
    QDBusConnectionInterface *interface = QDBusConnection::sessionBus().interface();

    QDBusReply<QStringList> serviceNames = interface->registeredServiceNames();
    if (serviceNames.isValid()) {
        dbusServices = serviceNames.value();
    }
    else {
        qDebug() << "Error receiving services:" << serviceNames.error().message();
    }
    connect(interface, SIGNAL(serviceRegistered(const QString &)), SLOT(onServiceRegistered(const QString &)));
    connect(interface, SIGNAL(serviceUnregistered(const QString &)), SLOT(onServiceUnregistered(const QString &)));
}

void DBusConnector::onServiceRegistered(const QString &name)
{
    qDebug() << "DBus service online:" << name;
    if (!dbusServices.contains(name)) dbusServices.append(name);
}

void DBusConnector::onServiceUnregistered(const QString &name)
{
    qDebug() << "DBus service offline:" << name;
    if (dbusServices.contains(name)) dbusServices.removeAll(name);
}
