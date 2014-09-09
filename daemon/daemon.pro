TARGET = harbour-systemmonitord

CONFIG += console
CONFIG += link_pkgconfig

QT -= gui
QT += dbus sql

PKGCONFIG += mlite5 keepalive
QMAKE_CXXFLAGS += -std=c++0x

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    daemon.cpp \
    dbusadapter.cpp \
    dbusconnector.cpp \
    service.cpp \
    storage.cpp \
    datasource.cpp \
    datasourcecpu.cpp \
    datasourcebattery.cpp \
    datasourcewlan.cpp \
    datasourcecell.cpp

HEADERS += \
    settings.h \
    dbusadapter.h \
    dbusconnector.h \
    service.h \
    storage.h \
    datasource.h \
    datasourcecpu.h \
    datasourcebattery.h \
    datasourcewlan.h \
    datasourcecell.h

#OTHER_FILES += \
#    net.thecust.systemmonitord.xml

INSTALLS += target sysmond

target.path = /usr/bin

sysmond.files = $${TARGET}.service
sysmond.path = /usr/lib/systemd/user

#qtcreator is bugged
INCLUDEPATH += $$[QT_HOST_PREFIX]/include/mlite5
INCLUDEPATH += $$[QT_HOST_PREFIX]/include/keepalive
