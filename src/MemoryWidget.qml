import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Frame {
    id: root
    padding: 5
    Layout.fillWidth: true
    Layout.fillHeight: true

    property real total
    property real used
    property real available
    property real cached
    property real free
    property real swapTotal
    property real swapUsed

    readonly property string totalString: stringify(total)
    readonly property string usedString: stringify(used)
    readonly property string availableString: stringify(available)
    readonly property string cachedString: stringify(cached)
    readonly property string freeString: stringify(free)
    readonly property string swapTotalString: stringify(swapTotal)
    readonly property string swapUsedString: stringify(swapUsed)
    readonly property real usedNormalized: used / total
    readonly property real availableNormalized: available / total
    readonly property real cachedNormalized: cached / total
    readonly property real freeNormalized: free / total
    readonly property real swapNormalized: swapUsed / swapTotal

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
        readonly property int leftWidth: 70
        readonly property int rightWidth: 70
        readonly property int barHeight: 10
        readonly property int labelHeight: 16
        readonly property color barUsedBG: '#fee'
        readonly property color barFreeBG: '#eef'
        readonly property color barUsedFG: '#f99'
        readonly property color barFreeFG: '#99f'
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        Item {
            Layout.fillWidth: true
            height: 16
            Label {
                anchors.centerIn: parent
                text: 'Memory usage'
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Label {
                Layout.fillWidth: true
                text: 'Total:'
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
                    text: 'avail: ' + stringifyPercent(root.available, root.total);
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
                    width: parent.width * (root.availableNormalized)
                    color: _.barFreeFG
                }
            }
            Item {
                width: _.rightWidth
                height: _.labelHeight
                Label {
                    anchors.right: parent.right
                    horizontalAlignment: Label.AlignRight
                    text: root.availableString
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Item {
                width: _.leftWidth
                height: _.labelHeight
                Label {
                    text: 'cached: ' + stringifyPercent(root.cached, root.total);
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
                    width: parent.width * (root.cachedNormalized)
                    color: _.barFreeFG
                }
            }
            Item {
                width: _.rightWidth
                height: _.labelHeight
                Label {
                    anchors.right: parent.right
                    horizontalAlignment: Label.AlignRight
                    text: root.cachedString
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
        RowLayout {
            Layout.fillWidth: true
            Item {
                width: _.leftWidth
                height: _.labelHeight
                Label {
                    text: 'swap: ' + stringifyPercent(root.swapUsed, root.swapTotal);
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
                    width: parent.width * (root.swapNormalized)
                    color: _.barFreeFG
                }
            }
            Item {
                width: _.rightWidth
                height: _.labelHeight
                Label {
                    anchors.right: parent.right
                    horizontalAlignment: Label.AlignRight
                    text: root.swapUsedString
                }
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
