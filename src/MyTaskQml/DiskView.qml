import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import 'Common.js' as Common

Pane {
    id: root
    Layout.fillWidth: true
    padding: 5
    topPadding: 2
    bottomPadding: 2

    property var disk

    background: Rectangle {
        color: Common.diskPaneBG
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            Label {
                Layout.fillWidth: true
                text: root.disk.name
            }
            Label {
                width: Common.rightWidth
                text: Common.stringifyMemory(root.disk.total)
            }
        }
        MemoryBar {
            title: 'used'
            value: root.disk.used
            total: root.disk.total
        }
        MemoryBar {
            title: 'avail'
            value: root.disk.available
            total: root.disk.total
        }
        MemoryBar {
            title: 'free'
            value: root.disk.free
            total: root.disk.total
        }
    }
}
