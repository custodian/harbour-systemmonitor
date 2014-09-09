import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0

CoverBackground {
    id: cover

    property int coverGraphHeight: cover.height * 0.5

    property bool needUpdate: true
    property bool active: status == Cover.Active
    onActiveChanged: {
        if (active) {
            updateGraph();
        }
    }

    ConfigurationGroup {
        id: settings
        path: "/net/thecust/systemmonitor"

        property int coverGraphNum: 0
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
            console.log("Conections dataUpdated");
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
                    return wlanGraph;
                case 2:
                    return cellGraph;
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

        GraphData {
            graphTitle: qsTr("CPU")
            graphHeight: coverGraphHeight
            gridX: 1
            minY: 0
            maxY: 100
            valueConverter: function(value) {
                return value.toFixed(1);
            }
            clickEnabled: false

            function updateGraph() {
                setPoints(sysmon.cpuTotal(1, graphWidth));
            }
        }
    }

    Component {
        id: wlanGraph
        GraphData {
            id: graphWlanTotal
            graphTitle: qsTr("Wlan")
            graphHeight: coverGraphHeight
            gridX: 1
            scale: true
            unitsY: "Kb"
            valueConverter: function(value) {
                return (value/1000).toFixed(0);
            }
            clickEnabled: false

            function updateGraph() {
                setPoints(sysmon.wlanTotal(1, graphWidth));
            }
        }
    }

    Component {
        id: cellGraph

        GraphData {
            id: graphCellTotal
            graphTitle: qsTr("Cell")
            graphHeight: coverGraphHeight
            gridX: 1
            scale: true
            unitsY: "Kb"
            valueConverter: function(value) {
                return (value/1000).toFixed(0);
            }
            clickEnabled: false

            function updateGraph() {
                setPoints(sysmon.cellTotal(1, graphWidth));
            }
        }
    }

    Component {
        id: batteryGraph

        GraphData {
            graphTitle: qsTr("BAT")
            graphHeight: coverGraphHeight
            gridX: 1
            minY: 0
            maxY: 100
            valueConverter: function(value) {
                return value.toFixed(2);
            }

            clickEnabled: false

            function updateGraph() {
                setPoints(sysmon.batteryCharge(1, graphWidth));
            }
        }
    }
}


