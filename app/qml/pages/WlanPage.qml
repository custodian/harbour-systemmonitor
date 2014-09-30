import QtQuick 2.0
import Sailfish.Silica 1.0
import net.thecust.sysmon 1.0

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
        wlanRx.updateGraph();
        wlanTx.updateGraph();
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
                title: qsTr("Wlan networks")
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

            SysMonGraph {
                id: wlanRx
                graphTitle: qsTr("Received data")
                graphHeight: 200
                dataType: [DataSource.NetworkWlanRx]
                scale: true
                axisY.units: "Kb"
                valueTotal: true
                valueConverter: function(value) {
                    return (value/1000).toFixed(0);
                }

                clickEnabled: false
            }

            SysMonGraph {
                id: wlanTx
                graphTitle: qsTr("Transmitted data")
                graphHeight: 200
                dataType: [DataSource.NetworkWlanTx]
                scale: true
                axisY.units: "Kb"
                valueTotal: true
                valueConverter: function(value) {
                    return (value/1000).toFixed(0);
                }

                clickEnabled: false
            }
        }
    }
}
