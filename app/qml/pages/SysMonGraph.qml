import QtQuick 2.0
import "."

GraphData {
    property QtObject dataSource: sysmon
    property var dataType: []
    property int dataDepth: settings.deepView
    property bool dataAvg: false

    //TODO: really such a thing?
    onDataAvgChanged: {
        valueTotal = !dataAvg;
    }

    function updateGraph() {
        var dataPoints = dataSource.getSystemGraph(dataType, dataDepth, graphWidth, dataAvg);
        setPoints(dataPoints);
    }
}
