import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import 'Common.js' as Common
import my.rimac.task 1.0

Pane {
    id: root
    padding: 5
    Layout.minimumWidth: 100
    Layout.minimumHeight: 100
    Layout.fillWidth: true
    Layout.fillHeight: true

    readonly property string title: 'Cpu load'
    readonly property var cpu: ResourceMonitor.statistics.cpu
    readonly property var coreLoad: cpu.coreLoad

    onCoreLoadChanged: {
        if (coreLoadModel.count === 0) {
            root.coreLoad.forEach((e, i) => {
                coreLoadModel.append({ 'load': e });
            })
        } else {
            root.coreLoad.forEach((e, i) => {
                coreLoadModel.setProperty(i, 'load', e);
            })
        }
    }

    ListModel {
        id: coreLoadModel
    }

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
                value: root.cpu.load
                RowLayout {
                    anchors.fill: parent
                    spacing: 5
                    opacity: 0.7
                    Repeater {
                        model: coreLoadModel
                        CpuBar {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            value: load
                            color: Qt.lighter(Common.barFG)
                        }
                    }
                }
                Label {
                    anchors.centerIn: parent
                    text: (100 * cpu.load).toFixed(0) + '%'
                    font.pixelSize: Math.min(parent.width * 0.3, parent.height * 0.3)
                }
            }
        }
    }
}
