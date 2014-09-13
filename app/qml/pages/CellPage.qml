import QtQuick 2.0
import Sailfish.Silica 1.0

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
        cellRx.setPoints(sysmon.cellRx(deepView, cellRx.graphWidth));
        cellTx.setPoints(sysmon.cellTx(deepView, cellTx.graphWidth));
    }

    Connections {
        target: sysmon
        onDataUpdated: {
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
                title: qsTr("Cell networks")
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
                id: cellRx
                graphTitle: qsTr("Received data")
                graphHeight: 200
                scale: true
                axisY.units: "Kb"
                valueConverter: function(value) {
                    return (value/1000).toFixed(0);
                }

                clickEnabled: false
            }

            GraphData {
                id: cellTx
                graphTitle: qsTr("Transmitted data")
                graphHeight: 200
                scale: true
                axisY.units: "Kb"
                valueConverter: function(value) {
                    return (value/1000).toFixed(0);
                }

                clickEnabled: false
            }
        }
    }
}
