import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0

Page {
    id: page

    property int deepView: 12
    onDeepViewChanged: {
        for (var i=0;i<depthModel.count;i++) {
            if (depthModel.get(i).interval == deepView) {
                comboBoxDepthView.currentIndex = i;
                break;
            }
        }
        updateGraph()
    }

    function updateGraph() {
        graphCpu.setPoints(sysmon.cpuTotal(deepView, graphCpu.graphWidth));
        graphBattery.setPoints(sysmon.batteryCharge(deepView, graphBattery.graphWidth));
        graphWlanRx.setPoints(sysmon.wlanRx(deepView, graphWlanRx.graphWidth));
        graphCellRx.setPoints(sysmon.cellRx(deepView, graphCellRx.graphWidth));
    }

    Connections {
        target: sysmon
        onDataUpdated: {
            console.log("Conections dataUpdated");
            updateGraph();
        }
    }

    Component.onCompleted: {
        deepViewChanged();
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: column.height

        VerticalScrollDecorator { flickable: flickable }

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("System monitor")
            }

            ComboBox {
                id: comboBoxDepthView
                label: qsTr("Show data for")
                menu: ContextMenu {
                    Repeater {
                        model: depthModel
                        delegate: MenuItem {
                            text: model.label
                            onClicked: {
                                page.deepView = model.interval
                            }
                        }
                    }
                }
            }

            GraphData {
                id: graphBattery
                graphTitle: qsTr("Battery charge")
                graphHeight: 200
                minY: 0
                maxY: 100
                valueConverter: function(value) {
                    return value.toFixed(2);
                }

                onClicked: pageStack.push(Qt.resolvedUrl("BatteryPage.qml"), {deepView: page.deepView})
            }

            GraphData {
                id: graphCpu
                graphTitle: qsTr("CPU usage")
                graphHeight: 200
                minY: 0
                maxY: 100
                valueConverter: function(value) {
                    return value.toFixed(1);
                }

                onClicked: pageStack.push(Qt.resolvedUrl("CpuPage.qml"), {deepView: page.deepView})
            }

            GraphData {
                id: graphWlanRx
                graphTitle: qsTr("Wlan-data received")
                graphHeight: 200
                scale: true
                unitsY: "Kb"
                valueConverter: function(value) {
                    return (value/1000).toFixed(0);
                }

                onClicked: pageStack.push(Qt.resolvedUrl("WlanPage.qml"), {deepView: page.deepView})
            }

            GraphData {
                id: graphCellRx
                graphTitle: qsTr("Cell-data received")
                graphHeight: 200
                scale: true
                unitsY: "Kb"
                valueConverter: function(value) {
                    return (value/1000).toFixed(0);
                }

                onClicked: pageStack.push(Qt.resolvedUrl("CellPage.qml"), {deepView: page.deepView})
            }
        }
    }
}
