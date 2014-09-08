import QtQuick 2.0
import Sailfish.Silica 1.0
import "."

CoverBackground {
    id: cover

    property bool active: status == Cover.Active
    onActiveChanged: {
        if (active) {
            updateGraph();
        }
    }

    //TODO: save load from dconf
    property int graphNum: 1
    function nextGraph() {
        if (graphNum == 2) {
            graphNum = 1;
        } else {
            graphNum++;
        }
    }

    function updateGraph() {
        currentGraph.item.updateGraph();
    }

    Connections {
        target: sysmon
        onDataUpdated: {
            console.log("Conections dataUpdated");
            if (active) {
                updateGraph();
            }
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
                switch(graphNum){
                case 1:
                    return cpuGraph;
                case 2:
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
            graphHeight: cover.height * 0.5
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
        id: batteryGraph

        GraphData {
            graphTitle: qsTr("BAT")
            graphHeight: cover.height * 0.5
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


