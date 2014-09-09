import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0

Page {
    id: page

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
                height: Theme.itemSizeSmall
                width: parent.width
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Clear all data")
                onClicked: {
                    remorse.execute("Clearing archive data", function() {
                        sysmon.clearData();
                    });
                }
            }

        }
    }
}
