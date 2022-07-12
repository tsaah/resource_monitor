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
                    text: Common.stringifyMemory(Memory.memoryTotal)
                }
            }
            MemoryBar {
                title: 'used'
                value: Memory.memoryUsed
                total: Memory.memoryTotal
            }
            MemoryBar {
                title: 'avail'
                value: Memory.memoryAvailable
                total: Memory.memoryTotal
            }
            MemoryBar {
                title: 'cached'
                value: Memory.memoryCached
                total: Memory.memoryTotal
            }
            MemoryBar {
                title: 'free'
                value: Memory.memoryFree
                total: Memory.memoryTotal
            }
            MemoryBar {
                title: 'swap'
                value: Memory.swapUsed
                total: Memory.swapTotal
            }
        }
    }
}
