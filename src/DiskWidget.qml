import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Frame {
    id: root
    padding: 5
    Layout.fillWidth: true
    visible: total !== 0

    property string name
    property real total
    property real used
    property real free

    readonly property string totalString: stringify(total)
    readonly property string usedString: stringify(used)
    readonly property string freeString: stringify(free)
    readonly property real usedNormalized: used / total
    readonly property real freeNormalized: free / total

    property var stringify: function(value) {
        let i = 0;
        while (value >= 1024) {
            value /= 1024;
            ++i;
        }
        let unit = [ 'B', 'KiB', 'MiB', 'Gib', 'TiB' ];
        return value.toFixed(2) + ' ' + unit[i];
    }

    property var stringifyPercent: function(value, totalValue) {
        return (100 * value / totalValue).toFixed(0) + '%';
    }


    QtObject {
        id: _
        readonly property int leftWidth: 60
        readonly property int rightWidth: 70
        readonly property int barHeight: 10
        readonly property int labelHeight: 16
        readonly property color barUsedBG: '#fee'
        readonly property color barFreeBG: '#efe'
        readonly property color barUsedFG: '#f99'
        readonly property color barFreeFG: '#9f9'
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            Layout.fillWidth: true
            Label {
                Layout.fillWidth: true
                text: root.name
            }
            Label {
                width: _.rightWidth
                text: root.totalString
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Item {
                width: _.leftWidth
                height: _.labelHeight
                Label {
                    text: 'used: ' + stringifyPercent(root.used, root.total);
                }
            }
            Rectangle {
                Layout.fillWidth: true
                height: _.barHeight
                color: _.barUsedBG
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width * (root.usedNormalized)
                    color: _.barUsedFG
                }
            }
            Item {
                width: _.rightWidth
                height: _.labelHeight
                Label {
                    anchors.right: parent.right
                    horizontalAlignment: Label.AlignRight
                    text: root.usedString
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Item {
                width: _.leftWidth
                height: _.labelHeight
                Label {
                    text: 'free: ' + stringifyPercent(root.free, root.total);
                }
            }
            Rectangle {
                Layout.fillWidth: true
                height: _.barHeight
                color: _.barFreeBG
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width * (root.freeNormalized)
                    color: _.barFreeFG
                }
            }
            Item {
                width: _.rightWidth
                height: _.labelHeight
                Label {
                    anchors.right: parent.right
                    horizontalAlignment: Label.AlignRight
                    text: root.freeString
                }
            }
        }
    }
}

