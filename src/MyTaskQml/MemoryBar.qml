import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import 'Common.js' as Common

RowLayout {
    id: root
    Layout.fillWidth: true

    property string title
    property real value
    property real total

    Behavior on value {
        PropertyAnimation {
            easing.type: Easing.OutExpo
            duration: Common.animationDuration
        }
    }

    RowLayout {
        Label {
            Layout.minimumWidth: Common.firstWidth
            text: root.title + ':'
        }
        Label {
            Layout.minimumWidth: Common.secondWidth
            horizontalAlignment: Label.AlignRight
            text: Common.stringifyPercent(root.value, root.total);
        }
    }
    Rectangle {
        Layout.fillWidth: true
        height: Common.barHeight
        color: Common.barBG
        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width * (root.value / root.total)
            color: Common.barFG
        }
    }
    Label {
        Layout.minimumWidth: Common.rightWidth
        horizontalAlignment: Label.AlignRight
        text: Common.stringifyMemory(root.value)
    }
}
