import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import Macai.App 1.0

Window {
    id: window
    visible: true
    width: 1200
    height: 600
    title: qsTr("QML TableView example")
    onWidthChanged: {
        tableView.forceLayout()
    }

    TableView {
        id: tableView
        anchors.fill: parent
        columnSpacing: 0
        rowSpacing: 0
        clip: true
        leftMargin: vericalHeader.implicitWidth
        topMargin: horizontalHeader.implicitHeight
        rowHeightProvider: function (row) { return 32; }
        columnWidthProvider: function (column) {
            return Math.max(1, (tableView.width - leftMargin) / tableView.columns)
        }

        ScrollIndicator.horizontal: ScrollIndicator { }
        ScrollIndicator.vertical: ScrollIndicator { }

        model: SqlTableModel {
            id: tableModel
        }

        delegate: Rectangle {
            id: cellItem

            implicitWidth: content.implicitHeight
            implicitHeight: 30
            Rectangle { anchors.left: parent.left; height: parent.height; width: 1; color: "#dddddd"}
            Rectangle { anchors.top: parent.top; width: parent.width; height: 1; color: "#dddddd"}
            Rectangle { anchors.right: parent.right; height: parent.height; width: 1; color: "#dddddd"; visible: model.column === tableView.columns -1 }
            Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: "#dddddd"; visible: model.row === tableView.rows - 1 }

            Text {
                id: content
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                padding: 4
                clip: true
                text: tableModel.data(tableModel.index(row, column))
            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: cellItem.color = "lightsteelblue"
                onExited: cellItem.color = "#ffffff"
                onClicked: {
                    console.log("(", row, ",", column ,")", "[", cellItem.x, ",",cellItem.y, "]")
                    console.log(tableModel.rowCount(), tableModel.columnCount())
                }
            }
        }

        Button {
            z: 3
            y: tableView.contentY
            x: tableView.contentX
            text: "#"
            width: tableView.leftMargin
            height: tableView.topMargin
        }

        Row {
            id: horizontalHeader
            y: tableView.contentY
            z: 2
            Repeater {
                model: tableView.columns > 0 ? tableView.columns : 1

                Button {
                    width: tableView.columnWidthProvider(index)
                    height: tableView.rowHeightProvider(index)
                    text: tableModel.headerData(index, Qt.Horizontal)
                }
            }
        }

        Column {
            id: vericalHeader
            x: tableView.contentX
            z: 2
            Repeater {
                model: tableView.rows > 0 ? tableView.rows : 1
                Button {
                    width: 30
                    height: tableView.rowHeightProvider(index)
                    text: tableModel.headerData(index, Qt.Vertical)
                }
            }
        }
    }

    Component {
        id: highlightComponent
        Rectangle {
            id: highlightRect
            width: 200; height: 50
            color: "#FFFF88"
            Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
        }
    }
}
