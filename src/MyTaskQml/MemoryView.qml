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

    readonly property string title: 'Memory usage'
    readonly property var memory: ResourceMonitor.statistics.memory

    ColumnLayout {
        anchors.left:  parent.left
        anchors.right: parent.right
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: root.title
        }
        ColumnLayout {
            spacing: 0
            RowLayout {
                Layout.fillWidth: true
                Label {
                    Layout.fillWidth: true
                    text: 'Total:'
                }
                Label {
                    Layout.minimumWidth: Common.rightWidth
                    horizontalAlignment: Label.AlignRight
                    text: Common.stringifyMemory(root.memory.memoryTotal)
                }
            }
            MemoryBar {
                title: 'used'
                value: root.memory.memoryUsed
                total: root.memory.memoryTotal
            }
            MemoryBar {
                title: 'avail'
                value: root.memory.memoryAvailable
                total: root.memory.memoryTotal
            }
            MemoryBar {
                title: 'cached'
                value: root.memory.memoryCached
                total: root.memory.memoryTotal
            }
            MemoryBar {
                title: 'free'
                value: root.memory.memoryFree
                total: root.memory.memoryTotal
            }
            MemoryBar {
                title: 'swap'
                value: root.memory.swapUsed
                total: root.memory.swapTotal
            }
        }
    }
}
