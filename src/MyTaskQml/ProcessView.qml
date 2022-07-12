import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import my.rimac.task 1.0

Pane {
    id: root
    padding: 5
    Layout.fillHeight: true
    Layout.minimumWidth: 130

    readonly property string title: 'Process info'
    readonly property var process: ResourceMonitor.statistics.process

    ColumnLayout {
        anchors.left:  parent.left
        anchors.right: parent.right
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: root.title
        }
        ColumnLayout {
            spacing: 0
            Label {
                text: 'Process count: ' + root.process.processCount
            }
            Label {
                text: 'Thread count: ' + root.process.threadCount
            }
        }
    }
}
