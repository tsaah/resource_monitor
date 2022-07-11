import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import my.rimac.task 1.0

Window {
    id: root
    visible: true
    width: 1200
    height: Math.max(ResourceMonitor.statistics.disk.disksOrder.length * 70 + 25, 250)
    title: 'Resource monitor'

    RowLayout {
        anchors.fill: parent
        spacing: 2
        CpuWidget {
            load: ResourceMonitor.statistics.cpu.load
            coreLoad: ResourceMonitor.statistics.cpu.coreLoad
        }
        ProcessWidget {
            processCount: ResourceMonitor.statistics.process.processCount
            threadCount: ResourceMonitor.statistics.process.threadCount
        }
        MemoryWidget {
            total: ResourceMonitor.statistics.memory.memoryTotal
            free: ResourceMonitor.statistics.memory.memoryFree
            available: ResourceMonitor.statistics.memory.memoryAvailable
            used: ResourceMonitor.statistics.memory.memoryUsed
            cached: ResourceMonitor.statistics.memory.memoryCached
            swapTotal: ResourceMonitor.statistics.memory.swapTotal
            swapUsed: ResourceMonitor.statistics.memory.swapUsed
        }
        ColumnLayout {
            Layout.fillWidth: true
            Frame {
                Layout.fillWidth: true
                Layout.fillHeight: true
                padding: 5

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 2
                    Item {
                        Layout.fillWidth: true
                        height: 16
                        Label {
                            anchors.centerIn: parent
                            text: 'Disk usage'
                        }
                    }
                    Repeater {
                        model: ResourceMonitor.statistics.disk.disksOrder
                        DiskWidget {
                            name: ResourceMonitor.statistics.disk.infos[modelData].name
                            total: ResourceMonitor.statistics.disk.infos[modelData].total
                            used: ResourceMonitor.statistics.disk.infos[modelData].used
                            free: ResourceMonitor.statistics.disk.infos[modelData].free
                        }
                    }
                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }


//    property string memoryTotal: (ResourceMonitor.statistics.memory.memoryTotal / 1024 / 1024 / 1024).toFixed(2) + ' Gb'
//    property string swapTotal: (ResourceMonitor.statistics.memory.swapTotal / 1024 / 1024 / 1024).toFixed(2) + ' Gb'
//    ColumnLayout {
//        anchors.fill: parent
//        Label {
//            text: 'core count: ' + ResourceMonitor.statistics.cpu.coreCount
//        }
//        Label {
//            text: 'process count: ' + ResourceMonitor.statistics.process.processCount
//        }
//        Label {
//            text: 'thread count: ' + ResourceMonitor.statistics.process.threadCount
//        }


//        ColumnLayout {
//            anchors.fill: parent
//            spacing: 2
//            Repeater {
//                model: ResourceMonitor.statistics.disk.disksOrder
//                DiskWidget {
//                    name: ResourceMonitor.statistics.disk.infos[modelData].name
//                    total: ResourceMonitor.statistics.disk.infos[modelData].total
//                    used: ResourceMonitor.statistics.disk.infos[modelData].used
//                    free: ResourceMonitor.statistics.disk.infos[modelData].free
//                }
//            }
//            Item {
//                Layout.fillHeight: true
//            }
//        }

//        CpuWidget {
//            load: ResourceMonitor.statistics.cpu.load
//            coreLoad: ResourceMonitor.statistics.cpu.coreLoad
//        }

//        ProgressBar {
//            to: ResourceMonitor.statistics.memory.memoryTotal
//            value: ResourceMonitor.statistics.memory.memoryFree
//            Label {
//                z: 10
//                text: (parent.value / 1024 / 1024 / 1024).toFixed(2) + ' Gb / ' + root.memoryTotal
//            }
//        }
//        ProgressBar {
//            to: ResourceMonitor.statistics.memory.memoryTotal
//            value: ResourceMonitor.statistics.memory.memoryAvailable
//            Label {
//                z: 10
//                text: (parent.value / 1024 / 1024 / 1024).toFixed(2) + ' Gb / ' + root.memoryTotal
//            }
//        }
//        ProgressBar {
//            to: ResourceMonitor.statistics.memory.memoryTotal
//            value: ResourceMonitor.statistics.memory.memoryUsed
//            Label {
//                z: 10
//                text: (parent.value / 1024 / 1024 / 1024).toFixed(2) + ' Gb / ' + root.memoryTotal
//            }
//        }
//        ProgressBar {
//            to: ResourceMonitor.statistics.memory.memoryTotal
//            value: ResourceMonitor.statistics.memory.memoryCached
//            Label {
//                z: 10
//                text: (parent.value / 1024 / 1024 / 1024).toFixed(2) + ' Gb / ' + root.memoryTotal
//            }
//        }
//        ProgressBar {
//            to: ResourceMonitor.statistics.memory.swapTotal
//            value: ResourceMonitor.statistics.memory.swapUsed
//            visible: to > 0
//            Label {
//                z: 10
//                text: (parent.value / 1024 / 1024 / 1024).toFixed(2) + ' Gb / ' + root.swapTotal
//            }
//        }


//        RowLayout {
//            spacing: 1
//            Rectangle {
//                Layout.leftMargin: 10
//                height: 200
//                width: 10
//                color:'#eef'

//                Rectangle {
//                    width: parent.width
//                    height: parent.height * ResourceMonitor.statistics.cpu.load
//                    anchors.bottom: parent.bottom
//                    color: '#aaf'
//                    Behavior on height {
//                        PropertyAnimation {
//                            easing.type: Easing.InCubic
//                            duration: 100
//                        }
//                    }
//                }
//            }

//            Repeater {
//                model: ResourceMonitor.statistics.cpu.coreLoad

//                Rectangle {
//                    Layout.leftMargin: 0
//                    height: 200 / 4
//                    width: 4
//                    color:'#fef'


//                    Rectangle {
//                        width: parent.width
//                        height: parent.height * modelData
//                        anchors.bottom: parent.bottom
//                        color: '#faf'
//                        Behavior on height {
//                            PropertyAnimation {
//                                easing.type: Easing.OutCubic
//                                duration: 100
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
}

