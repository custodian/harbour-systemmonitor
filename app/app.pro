TARGET = harbour-systemmonitor

CONFIG += sailfishapp
PKGCONFIG += mlite5

QT += dbus sql
QMAKE_CXXFLAGS += -std=c++0x

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    app.cpp \
    ..\daemon\storage.cpp \
    ..\daemon\datasource.cpp \
    ..\daemon\systemsnapshot.cpp \
    systemmonitor.cpp

HEADERS += \
    ..\daemon\settings.h \
    ..\daemon\storage.h \
    ..\daemon\datasource.h \
    ..\daemon\systemsnapshot.h \
    systemmonitor.h

OTHER_FILES += \
    qml/pages/CoverPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/CpuPage.qml \
    qml/pages/RamPage.qml \
    qml/pages/WlanPage.qml \
    qml/pages/CellPage.qml \
    qml/pages/BatteryPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/Axis.qml \
    qml/pages/GraphData.qml \
    qml/sysmon.qml \
    qml/images/* \
    harbour-systemmonitor.desktop \
    harbour-systemmonitor.png

