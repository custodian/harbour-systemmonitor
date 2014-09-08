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
        cpuTotal.setPoints(sysmon.cpuTotal(deepView, cpuTotal.graphWidth));
        cpuUser.setPoints(sysmon.cpuUser(deepView, cpuUser.graphWidth));
        cpuSystem.setPoints(sysmon.cpuSystem(deepView, cpuSystem.graphWidth));
        cpuIO.setPoints(sysmon.cpuIO(deepView, cpuIO.graphWidth));
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

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("CPU statistics")
            }

            ComboBox {
                id: comboBoxDepthView
                label: qsTr("Show data for")
                currentIndex: 2
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
                id: cpuTotal
                graphTitle: qsTr("Total")
                graphHeight: 200
                minY: 0
                maxY: 100
                valueConverter: function(value) {
                    return value.toFixed(1);
                }

                clickEnabled: false
            }

            GraphData {
                id: cpuUser
                graphTitle: qsTr("User processes")
                graphHeight: 200
                minY: 0
                maxY: 100
                valueConverter: function(value) {
                    return value.toFixed(1);
                }

                clickEnabled: false
            }

            GraphData {
                id: cpuSystem
                graphTitle: qsTr("System processes")
                graphHeight: 200
                minY: 0
                maxY: 100
                valueConverter: function(value) {
                    return value.toFixed(1);
                }

                clickEnabled: false
            }

            GraphData {
                id: cpuIO
                graphTitle: qsTr("IO wait")
                graphHeight: 200
                minY: 0
                maxY: 100
                valueConverter: function(value) {
                    return value.toFixed(1);
                }

                clickEnabled: false
            }
        }
    }
}
