import QtQuick 2.0
import QtQml 2.1
import Sailfish.Silica 1.0
import "."

Column {
    id: root
    anchors {
        left: (parent)? parent.left : undefined
        right: (parent)? parent.right : undefined
    }
    property alias clickEnabled: backgroundArea.enabled

    signal clicked

    //TODO: move all settings to Axis object
    //property Axis axisX
    //property Axis axisY
    property string graphTitle: ""
    property string maskY: "%1"
    property string unitsY: "%"
    property int gridY: 4
    property int gridX: 4

    property var valueConverter

    property string maskX: "hh:mm"
    property int graphHeight: 250
    property int graphWidth: canvas.width / lineWidth

    property bool scale: false
    property color lineColor: Theme.highlightColor
    property int lineWidth: 3

    property real minY: 0 //Always 0
    property real maxY: 0

    property int minX: 0
    property int maxX: 0

    property var points: []
    onPointsChanged: {
        noData = (points.length == 0);
    }
    property bool noData: true

    function setPoints(data) {
        var pointMaxY = 0;
        if (data.length > 0) {
            minX = data[0].x;
            maxX = data[data.length-1].x;
        }
        data.forEach(function(point) {
            if (point.y > pointMaxY) {
                pointMaxY = point.y
            }
        });
        points = data;
        if (scale) {
            maxY = pointMaxY * 1.20;
        }
        canvas.requestPaint();
    }

    function createYLabel(value) {
        var v = value;
        if (valueConverter) {
            v = valueConverter(value);
        }
        return maskY.arg(v);
    }

    function createXLabel(value) {
        var d = new Date(value*1000);
        return Qt.formatTime(d, maskX);
    }

    Label {
        anchors {
            left: parent.left
            leftMargin: 3*Theme.paddingLarge
            right: parent.right
            rightMargin: Theme.paddingLarge
        }
        color: Theme.highlightColor
        font.pixelSize: Theme.fontSizeSmall
        text: graphTitle
        wrapMode: Text.Wrap

        Label {
            id: labelLastValue
            anchors {
                right: parent.right
            }
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
            wrapMode: Text.Wrap
            visible: !noData
        }
    }

    Rectangle {
        anchors {
            left: parent.left
            leftMargin: 3*Theme.paddingLarge
            rightMargin: Theme.paddingLarge
            right: parent.right
        }
        height: graphHeight
        border.color: Theme.secondaryHighlightColor
        color: "transparent"

        BackgroundItem {
            id: backgroundArea
            anchors.fill: parent
            onClicked: {
                root.clicked();
            }
        }

        Repeater {
            model: noData ? 0 : (gridY + 1)
            delegate: Label {
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeLarge / 2
                text: createYLabel( (maxY-minY)/gridY * index + minY)
                anchors {
                    top: (index == gridY) ? parent.top : undefined
                    bottom: (index == gridY) ? undefined : parent.bottom
                    bottomMargin: (index) ? parent.height / gridY * index - height/2 : 0
                    right: parent.left
                    rightMargin: Theme.paddingSmall
                }
            }
        }

        Repeater {
            model: noData ? 0 : (gridX + 1)
            delegate: Label {
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeLarge / 2
                text: createXLabel( (maxX-minX)/gridX * index + minX )
                anchors {
                    top: parent.bottom
                    topMargin: Theme.paddingSmall
                    left: (index == gridX) ? undefined : parent.left
                    right: (index == gridX) ? parent.right : undefined
                    leftMargin: (index) ? (parent.width / gridX * index - width/2): 0
                }
            }
        }

        Label {
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeLarge / 2
            text: unitsY
            anchors {
                top: parent.top
                left: parent.left
                leftMargin: Theme.paddingSmall
            }
            visible: !noData
        }

        Canvas {
            id: canvas
            anchors {
                fill: parent
                //leftMargin: Theme.paddingSmall
                //rightMargin: Theme.paddingSmall
            }

            //renderTarget: Canvas.FramebufferObject
            //renderStrategy: Canvas.Threaded

            Component.onCompleted: {
                console.log("width:", width, "height:", height);
            }

            property real stepX: 1
            property real stepY: (maxY-minY)/(height-1)

            function drawGrid(ctx) {
                ctx.save();

                ctx.lineWidth = 1;
                ctx.strokeStyle = lineColor;
                ctx.globalAlpha = 0.4;
                //i=0 and i=gridY skipped, top/bottom line
                for (var i=1;i<gridY;i++) {
                    ctx.beginPath();
                    ctx.moveTo(0, height/gridY * i);
                    ctx.lineTo(width, height/gridY * i);
                    ctx.stroke();
                }

                ctx.restore();
            }

            function drawPoints(ctx, points) {

            }

            onPaint: {
                var ctx = canvas.getContext("2d");
                ctx.globalCompositeOperation = "source-over";
                ctx.clearRect(0,0,width,height);

                console.log("maxY", maxY, "minY", minY, "height", height, "StepY", stepY);


                var end = points.length;

                if (end > 0) {
                    drawGrid(ctx);
                }

                ctx.save()
                ctx.strokeStyle = lineColor;
                //ctx.globalAlpha = 0.8;
                ctx.lineWidth = lineWidth;
                ctx.beginPath();
                var x = 0;
                for (var i = 0; i < end; i++) {
                    var y = height - Math.floor(points[i].y / stepY) - 1;
                    if (i == 0) {
                        ctx.moveTo(x, y);
                    } else {
                        ctx.lineTo(x, y);
                    }
                    x+=lineWidth; //point[i].x can be used for grid title
                }
                ctx.stroke();
                ctx.restore();

                if (end > 0) {
                    labelLastValue.text = root.createYLabel(points[end-1].y)+root.unitsY;
                }
            }
        }

        Text {
            id: textNoData
            anchors.centerIn: parent
            color: lineColor
            text: qsTr("No data");
            visible: noData
        }
    }

    //Just for bottom offset for axis label
    Item {
        height: Theme.fontSizeSmall
        width: parent.width
    }
}
