import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import net.thecust.sysmon 1.0
import org.nemomobile.configuration 1.0

//TODO: combine all settings here

ApplicationWindow
{
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("pages/CoverPage.qml")

    ListModel {
        id: timeModel
        ListElement {
            label: "30 seconds"
            interval: 30
        }
        ListElement {
            label: "1 minute"
            interval: 60
        }
        ListElement {
            label: "2 minutes"
            interval: 120
        }
        ListElement {
            label: "3 minutes"
            interval: 180
        }
        ListElement {
            label: "5 minutes"
            interval: 300
        }
        ListElement {
            label: "10 minutes"
            interval: 600
        }
    }

    ListModel {
        id: archiveModel
        ListElement {
            label: "7 days"
            interval: 7
        }
        ListElement {
            label: "14 days"
            interval: 14
        }
    }

    ListModel {
        id: depthModel
        ListElement {
            label: "1 hour"
            interval:  1
        }
        ListElement {
            label: "4 hours"
            interval: 4
        }
        ListElement {
            label: "6 hours"
            interval: 6
        }
        ListElement {
            label: "8 hours"
            interval: 8
        }
        ListElement {
            label: "12 hours"
            interval: 12
        }
        ListElement {
            label: "24 hours"
            interval: 24
        }
        ListElement {
            label: "2 days"
            interval: 48
        }
        ListElement {
            label: "3 days"
            interval: 72
        }
        ListElement {
            label: "4 days"
            interval: 96
        }
        ListElement {
            label: "1 week"
            interval: 168
        }
        ListElement {
            label: "whole period"
            interval: 999
        }
    }

    SystemMonitor {
        id: sysmon

        /*onDataUpdated: {
            console.log("SystemMonitor dataUpdated");
        }*/
    }
}
