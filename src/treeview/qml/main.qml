import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import Macai.App 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt Quick 2.x TreeView example")

    TreeView {
        id: treeview

        anchors.left: parent.left
        height: parent.height
        width: 240

        model: JsonModel {
            id: jsonModel
            json: "file:./tree.json" // path to json file
        }
        delegate: ItemDelegate {
            id: delegateItem

            // inernal properties
            property var modelIndex: model["_TreeView_ModelIndex"]
            property bool expanded: model["_TreeView_ItemExpanded"]
            property bool hasChildren: model["_TreeView_HasChildren"]
            property int depth: model["_TreeView_ItemDepth"]
            property int indent: model["_TreeView_ItemDepth"] * 16 + spacing

            width: Math.max(parent.width, 180)

            RowLayout {
                anchors.fill: parent
                spacing: 4
                anchors.leftMargin: indent

                Image {
                    id: icon
                    source: "qrc:/resources/arrow.png"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    visible: delegateItem.hasChildren
                    rotation: delegateItem.expanded ? 90 : 0
                    Behavior on rotation { NumberAnimation { duration: 200; easing.type: Easing.OutQuad } }
                    MouseArea {
                        id: branchIconArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: icon.scale = 1.2
                        onExited: icon.scale = 1
                        onClicked: {
                            if (treeview.isExpanded(modelIndex)) {
                                treeview.collapse(modelIndex);
                                model.expanded = false;
                            } else {
                                treeview.expand(modelIndex);
                                model.expanded = true;
                            }
                        }
                    }
                }
                Text {
                    id: title
                    text: model.title || model.display || ""
                    maximumLineCount: 2
                    horizontalAlignment: Text.AlignLeft | Text.AlignVCenter
                    elide: Text.ElideRight
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillWidth: true
                }
            }

            onDoubleClicked: {
                if (treeview.isExpanded(modelIndex)) {
                    treeview.collapse(modelIndex);
                    model.expanded = false;
                } else {
                    treeview.expand(modelIndex);
                    model.expanded = true;
                }
            }
            onClicked: {
                treeview.currentIndex = index;
            }
            Component.onCompleted: {
                if(model.expanded)
                    treeview.expand(modelIndex);
            }
        } // ItemDelegate
    }
}
