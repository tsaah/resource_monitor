import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import my.rimac.task

Pane {
    id: root
    padding: 5
    Layout.fillHeight: true
    Layout.minimumWidth: 130

    readonly property string title: 'Process info'

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
                text: 'Process count: ' + Process.processCount
            }
            Label {
                text: 'Thread count: ' + Process.threadCount
            }
        }
    }
}
