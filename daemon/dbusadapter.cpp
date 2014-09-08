#include "dbusadapter.h"

#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QDebug>

DBusAdapter::DBusAdapter(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);

    QDBusConnection bus = QDBusConnection::sessionBus();
    qDebug() << "DBUS Register service" <<
    bus.registerService("net.thecust.systemmonitor");
    qDebug() << "DBUS Register object" <<
    bus.registerObject("/net/thecust/systemmonitor", parent);
}
