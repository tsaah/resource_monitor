import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import 'Common.js' as Common
import my.rimac.task

Pane {
    id: root
    padding: 5
    Layout.minimumWidth: 200
    Layout.fillWidth: true
    Layout.fillHeight: true

    readonly property string title: 'Disk usage'

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
                model: Disks.order
                DiskView {
                    disk: Disks.infos[modelData]
                }
            }
        }
    }
}
