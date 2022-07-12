import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import 'Common.js' as Common
import my.rimac.task 1.0

Pane {
    id: root
    padding: 5
    Layout.minimumWidth: 200
    Layout.fillWidth: true
    Layout.fillHeight: true

    readonly property string title: 'Disk usage'
    readonly property var disk: ResourceMonitor.statistics.disk

    ColumnLayout {
        anchors.left:  parent.left
        anchors.right: parent.right
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: root.title
        }
        ColumnLayout {
            spacing: 5
            Repeater {
                model: root.disk.disksOrder
                DiskView {
                    disk: root.disk.infos[modelData]
                }
            }
        }
    }
}
