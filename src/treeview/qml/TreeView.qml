import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0

import Macai.App 1.0

Item {
    id: control

    property var model: null
    property alias view: listView
    property alias delegate: listView.delegate
    property alias currentIndex: listView.currentIndex

    signal expanded(var index)
    signal collapsed(var index)
    signal activated(var index)
    signal clicked(var index)
    signal doubleClicked(var index)
    signal pressAndHold(var index)

    ListView {
        id: listView

        property string searchText: ""

        anchors.fill: parent
        clip: true
        currentIndex: -1
        highlightMoveDuration: 400
        highlight: Rectangle { color: "lightsteelblue"; opacity: 0.3 }
        ScrollBar.vertical: ScrollBar { id: verticalScrollbar }
        ScrollBar.horizontal: ScrollBar { }
        headerPositioning: ListView.PullBackHeader

        header: ToolBar {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 2
            anchors.rightMargin: verticalScrollbar.width
            implicitHeight: filterField.height < minimumHeight ? minimumHeight : filterField.height
            background: Rectangle { color: "red"; anchors.fill: parent }

            TextField {
                id: filterField
                width: parent.width
                implicitHeight: 30
                onTextChanged: {
                    listView.searchText = text;
                    // sortFilterModel.setFilterRegExp(text) qt5
                    // sortFilterModel.setFilterRegularExpression(text) qt6
                }
            }
        }

        model: TreeModelProxy {
            id: modelProxy
            onExpanded: control.expanded(index)
            onCollapsed: control.collapsed(index)
            model: SortFilterModel {
                id: sortFilterModel
                recursiveFilteringEnabled: true
                sourceModel: control.model
            }
        }

        add: Transition {
            NumberAnimation  { from: -control.width; to: 0; properties: "x"; duration: 300; easing.type: Easing.OutQuad }
        }
        remove: Transition {
            NumberAnimation { to: -control.width; property: "x"; duration: 300; easing.type: Easing.OutQuad }
        }
        displaced : Transition {
            NumberAnimation  { properties: "x,y"; duration: 300; easing.type: Easing.OutQuad }
        }
    }

    function mapToIndex(row) {
        return modelProxy.indexOf(row);
    }

    function isExpanded(index) {
//        if (index.valid && index.model !== model) {
//            console.warn("TreeView.isExpanded: model and index mismatch")
//            return false
//        }
        return modelProxy.isExpanded(index)
    }

    function collapse(index) {
//        if (index.valid && index.model !== model)
//            console.warn("TreeView.collapse: model and index mismatch")
//        else
            modelProxy.collapse(index)
    }

    function expand(index) {
//        if (index.valid && index.model !== model)
//            console.warn("TreeView.expand: model and index mismatch")
//        else
            modelProxy.expand(index)
    }

    function toggleExpand(index) {
//        if (index.valid && index.model !== model) {
//            console.warn("TreeView.expand: model and index mismatch")
//        }
//        else {
            if(isExpanded(index))
                collapse(index)
            else
                expand(index);
//        }
    }

    function setData(index, value) {
        modelProxy.setData(index, value);
    }

    function add(index) {
//        if (index.valid && index.model !== model) {
//            console.warn("TreeView.expand: model and index mismatch")
//            return null
//        }
//        else
            return modelProxy.add(index)
    }

    function remove(row, count) {
        return modelProxy.remove(row, count)
    }
}
