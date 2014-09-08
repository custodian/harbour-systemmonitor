#ifndef DBUSADAPTER_H
#define DBUSADAPTER_H

#include <QObject>
#include <QtDBus/QDBusAbstractAdaptor>

class QCoreApplication;

class DBusAdapter : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.thecust.systemmonitor")
public:
    explicit DBusAdapter(QObject *parent);

public slots:

signals:
    void dataUpdated();
};

#endif // DBUSADAPTER_H
