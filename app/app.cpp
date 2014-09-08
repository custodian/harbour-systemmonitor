#include <QtQuick>

#include <sailfishapp.h>
#include "systemmonitor.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<SystemMonitor>("net.thecust.sysmon", 1, 0, "SystemMonitor");
    qmlRegisterUncreatableType<DataSource>("net.thecust.sysmon", 1, 0, "DataSource", "Data Source type available only for enum datatypes");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));

    app->setOrganizationDomain("net");
    app->setOrganizationName("thecust");
    app->setApplicationName("SystemMonitor");

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->rootContext()->setContextProperty("APP_VERSION", APP_VERSION);
    view->setSource(SailfishApp::pathTo("qml/sysmon.qml"));
    view->show();

    return app->exec();
}

