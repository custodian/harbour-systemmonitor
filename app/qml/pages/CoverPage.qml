import QtQuick 2.0
import Sailfish.Silica 1.0
import net.thecust.sysmon 1.0

CoverBackground {
    id: cover

    property int coverGraphHeight: cover.height * 0.5

    property bool needUpdate: true
    property bool active: status == Cover.Active
    onActiveChanged: {
        updateGraph();
    }

    function nextGraph() {
        needUpdate = true;
        settings.coverGraphNum = (settings.coverGraphNum+1)%4
    }

    function updateGraph() {
        if (active && needUpdate) {
            currentGraph.item.updateGraph();
            needUpdate = false;
        }
    }

    Connections {
        target: sysmon
        onDataUpdated: {
            needUpdate = true;
            updateGraph();
        }
    }

    Column {
        width: parent.width
        anchors {
            top: parent.top
            topMargin: Theme.paddingMedium
        }
        spacing: Theme.paddingMedium

        Loader {
            id: currentGraph
            anchors {
                left: parent.left
                right: parent.right
            }
            sourceComponent: {
                switch(settings.coverGraphNum){
                case 0:
                    return cpuGraph;
                case 1:
                    return networkGraph;
                case 2:
                    return ramGraph;
                case 3:
                    return batteryGraph;

                }
            }
            onStatusChanged: {
                if (status == Loader.Ready) {
                    updateGraph();
                }
            }
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-m-levels"
            onTriggered: {
                nextGraph();
            }
        }
    }

    Component {
        id: cpuGraph

        SysMonGraph {
            graphTitle: qsTr("CPU")
            graphHeight: coverGraphHeight
            dataType: [DataSource.CpuTotal]
            dataDepth: 1
            dataAvg: true
            axisX.grid: 1
            minY: 0
            maxY: 100
            valueConverter: function(value) {
                return value.toFixed(1);
            }
            clickEnabled: false
        }
    }

    Component {
        id: networkGraph

        SysMonGraph {
            graphTitle: qsTr("NET")
            graphHeight: coverGraphHeight
            dataType: [DataSource.NetworkCellRx,
                DataSource.NetworkCellTx,
                DataSource.NetworkWlanRx,
                DataSource.NetworkWlanTx]
            dataDepth: 1
            scale: true
            axisX.grid: 1
            axisY.units: "Kb"
            valueConverter: function(value) {
                return (value/1000).toFixed(0);
            }
            clickEnabled: false
        }
    }

    Component {
        id: ramGraph

        SysMonGraph {
            graphTitle: qsTr("RAM")
            graphHeight: coverGraphHeight
            dataType: [DataSource.RAMUsed]
            dataDepth: 1
            dataAvg: true
            scale: true
            axisX.grid: 1
            axisY.units: "Mb"
            valueConverter: function(value) {
                return (value/1000).toFixed(0);
            }
            clickEnabled: false
        }
    }

    Component {
        id: batteryGraph

        SysMonGraph {
            graphTitle: qsTr("BAT")
            graphHeight: coverGraphHeight
            dataType: [DataSource.BatteryPercentage]
            dataDepth: 1
            dataAvg: true
            axisX.grid: 1
            minY: 0
            maxY: 100
            valueConverter: function(value) {
                return value.toFixed(2);
            }

            clickEnabled: false
        }
    }
}


