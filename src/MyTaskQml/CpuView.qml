import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import 'Common.js' as Common
import my.rimac.task

Pane {
    id: root
    padding: 5
    Layout.minimumWidth: 100
    Layout.minimumHeight: 100
    Layout.fillWidth: true
    Layout.fillHeight: true

    readonly property string title: 'CPU load'

    ColumnLayout {
        anchors.fill: parent
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: root.title
        }
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Common.barBG
            CpuBar {
                anchors.fill: parent
                value: Cpu.load
                RowLayout {
                    anchors.fill: parent
                    spacing: 5
                    opacity: 0.7
                    Repeater {
                        model: Cpu.coreCount
                        CpuBar {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            value: Cpu.coreLoad[index]
                            color: Qt.lighter(Common.barFG)
                        }
                    }
                }
                Label {
                    anchors.centerIn: parent
                    text: (100 * Cpu.load).toFixed(0) + '%'
                    font.pixelSize: Math.min(parent.width * 0.3, parent.height * 0.3)
                }
            }
        }
    }
}
