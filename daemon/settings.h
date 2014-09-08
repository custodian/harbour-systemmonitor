#ifndef SETTINGS_H
#define SETTINGS_H

#include <MDConfGroup>
#include <QDebug>

class Settings : public MDConfGroup
{
    Q_OBJECT

    Q_PROPERTY(int updateInterval MEMBER updateInterval NOTIFY updateIntervalChanged)
    Q_PROPERTY(int archiveLength MEMBER archiveLength NOTIFY archiveLengthChanged)
    //Q_PROPERTY(bool myProperty MEMBER myProperty NOTIFY myPropertyChanged)
public:
    //bool myProperty;
    int updateInterval;
    int archiveLength;

public:
    explicit Settings(QObject *parent = 0) :
        MDConfGroup("/net/thecust/systemmonitor", parent, BindProperties)
    {
        updateInterval = value("updateInterval", QVariant::fromValue(120)).toInt();
        archiveLength = value("archiveLength", QVariant::fromValue(7)).toInt();
        resolveMetaObject();
    }

signals:
    //void myPropertyChanged(bool);
    void updateIntervalChanged(int);
    void archiveLengthChanged(int);
};

#endif // SETTINGS_H
