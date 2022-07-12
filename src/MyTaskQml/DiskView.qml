import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
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
