import QtQuick
import 'Common.js' as Common

Item {
    id: root
    clip: true

    property alias color: bar.color
    property real value

    Behavior on value {
        PropertyAnimation {
            easing.type: Easing.OutExpo
            duration: Common.animationDuration
        }
    }

    Rectangle {
        id: bar
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: root.value * parent.height
        color: Common.barFG
    }
}
