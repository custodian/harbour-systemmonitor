#include <signal.h>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "service.h"
#include "settings.h"
#include "dbusconnector.h"

void signalhandler(int sig)
{
    switch (sig) {
    case SIGINT:
        qDebug() << "quit by SIGINT";
        qApp->quit();
        break;
    case SIGTERM:
        qDebug() << "quit by SIGTERM";
        qApp->quit();
        break;
    default:
        qDebug() << "unhandled signal" << sig;
        break;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setOrganizationDomain("net");
    app.setOrganizationName("thecust");
    app.setApplicationName("SystemMonitor");

    Settings settings;

    Service service(&app, &settings);

    signal(SIGINT, signalhandler);
    signal(SIGTERM, signalhandler);

    return app.exec();
}
