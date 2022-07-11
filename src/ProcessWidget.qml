import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Frame {
    id: root
    padding: 5
    Layout.fillHeight: true
    Layout.minimumWidth: 150

    property int processCount
    property int threadCount

    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            height: 16
            Label {
                anchors.centerIn: parent
                text: 'Process info'
            }
        }
        Item {
            Layout.fillWidth: true
            height: 16
            Label {
                text: 'Process count: ' + processCount
            }
        }
        Item {
            Layout.fillWidth: true
            height: 16
            Label {
                text: 'Thread count: ' + threadCount
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
