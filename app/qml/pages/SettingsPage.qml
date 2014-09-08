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
            console.log("Loaded interval", updateInterval);
            for (var i=0;i<timeModel.count;i++) {
                if (timeModel.get(i).interval == updateInterval) {
                    comboBoxUpdateInterval.currentIndex = i;
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
            spacing: Theme.paddingLarge
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

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Clear all data")
                onClicked: {
                    remorse.execute("Clearing data", function() {
                        sysmon.clearData();
                    });
                }
            }

        }
    }
}
