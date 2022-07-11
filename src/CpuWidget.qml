import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Frame {
    id: root
    padding: 5
    Layout.minimumWidth: 100
    Layout.fillHeight: true

    property real load
    property var coreLoad

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Item {
            Layout.fillWidth: true
            height: 16
            Label {
                anchors.centerIn: parent
                text: 'CPU load'
            }
        }

        Rectangle {
            id: totalLoadRect
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: '#eef'
            Rectangle {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                height: root.load * parent.height
                color: '#99f'
            }
            Label {
                anchors.centerIn: parent
                text: (100 * root.load).toFixed(0) + '%'
                font.pixelSize: Math.min(parent.width * 0.3, parent.height * 0.3)
            }
            RowLayout {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                spacing: 0
                Item {
                    Layout.fillWidth: true
                }
                Repeater {
                    model: root.coreLoad
                    Item {
                        height: totalLoadRect.height / root.coreLoad.length
                        width: totalLoadRect.width / root.coreLoad.length - 1
                        Rectangle {
                            width: parent.width
                            height: parent.height * modelData
                            anchors.bottom: parent.bottom
                            color: '#faf'
                            Behavior on height {
                                PropertyAnimation {
                                    easing.type: Easing.OutCubic
                                    duration: 100
                                }
                            }
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }
    }
}
