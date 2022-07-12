import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import my.rimac.task

Window {
    id: root
    visible: true
    width: 1200
    height: 400
    title: 'Resource monitor'
    color: Qt.lighter(Material.background)

    Component.onCompleted: ResourceMonitor.intervalMs = 100

    Material.theme: Material.Dark
    Material.primary: Qt.darker('#799ad4')
    Material.accent: '#799ad4'
    Material.background: '#36393d'
    Material.foreground: Qt.lighter('#b8c0cf')

    RowLayout {
        anchors.fill: parent
        spacing: 1
        CpuView {}
        ProcessView {}
        MemoryView {}
        DisksView {}
    }
}
