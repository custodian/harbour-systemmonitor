import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0

Page {
    id: page

    property int databaseSize: 0
    property int databaseUnits: 0

    Component.onCompleted: {
        updateDatabaseInfo();
    }

    function updateDatabaseInfo() {
        databaseUnits = sysmon.getUnitsCollected();
        databaseSize = sysmon.getDatabaseSize();
    }

    ConfigurationGroup {
        id: settings
        path: "/net/thecust/systemmonitor"
        property int updateInterval: 60
        onUpdateIntervalChanged: {
            for (var i=0;i<timeModel.count;i++) {
                if (timeModel.get(i).interval == updateInterval) {
                    comboBoxUpdateInterval.currentIndex = i;
                    break;
                }
            }
        }
        property int archiveLength: 7
        onArchiveLengthChanged: {
            for (var i=0;i<archiveModel.count;i++) {
                if (archiveModel.get(i).interval == archiveLength) {
                    comboBoxArchiveLength.currentIndex = i;
                    break;
                }
            }
        }
    }

    RemorsePopup { id: remorse }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: column.height

        VerticalScrollDecorator { flickable: flickable }

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingMedium
            PageHeader {
                title: qsTr("Settings")
            }

            ComboBox {
                id: comboBoxUpdateInterval
                label: qsTr("Update interval")
                currentIndex: 2
                menu: ContextMenu {
                    Repeater {
                        model: timeModel
                        delegate: MenuItem {
                            text: model.label
                            onClicked: {
                                settings.updateInterval = model.interval
                            }
                        }
                    }
                }
            }

            ComboBox {
                id: comboBoxArchiveLength
                label: qsTr("Data archive length")
                currentIndex: 0
                menu: ContextMenu {
                    Repeater {
                        model: archiveModel
                        delegate: MenuItem {
                            text: model.label
                            onClicked: {
                                settings.archiveLength = model.interval
                            }
                        }
                    }
                }
            }

            //Spacer
            Item {
                height: 1
                width: parent.width
            }

            Row {
                anchors {
                    left: parent.left
                    leftMargin: Theme.paddingLarge
                }
                spacing: Theme.paddingMedium
                Label {
                    text: qsTr("Data archive size")
                    color: Theme.highlightColor
                }
                Label {
                    text: qsTr("%1 Kb").arg(databaseSize/1000);
                    color: Theme.primaryColor
                }
            }
            Row {
                anchors {
                    left: parent.left
                    leftMargin: Theme.paddingLarge
                }
                spacing: Theme.paddingMedium
                Label {
                    text: qsTr("Units collected")
                    color: Theme.highlightColor
                }
                Label {
                    text: qsTr("%1").arg(databaseUnits);
                    color: Theme.primaryColor
                }
            }
            Text {
                width: parent.width
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
                text: qsTr("Cleaning data will erase currenly collected units, but will not reduce data archive size.\nShrink option will be added in future releases.")
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Clear all data")
                onClicked: {
                    remorse.execute("Clearing archive data", function() {
                        sysmon.clearData();
                        updateDatabaseInfo();
                    });
                }
            }
        }
    }
}
