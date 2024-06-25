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
            json: "qrc:/resources/tree.json" // path to json file
        }
        delegate: TreeItem {
            id: delegateItem

            text: model.title || model.display || ""

            onEditClicked: {
                treeview.currentIndex = index
            }
            onEditAccepted: {
                if(model.title !== undefined) {
                    if(text !== model.title) {
                        model.title = text
                    }
                } else {
                    if(text !== model.display) {
                        model.display = text
                    }
                }
            }
            onBranchIconClicked: {
                treeview.toggleExpand(modelIndex)
                model.expanded = !model.expanded
                treeview.currentIndex = index
            }
            onClicked: {
                treeview.currentIndex = index;
            }
            onAddClicked: {
                var i = treeview.add(modelIndex)
                if(i.valid) {
                    treeview.expand(modelIndex)
                    treeview.currentIndex = index + 1
                }
            }
            onRemoveClicked: {
                var ok = treeview.remove(index, 1)
                if(!ok) console.log("remove fail")
            }
            Component.onCompleted: {
                if(model.expanded)
                    treeview.expand(modelIndex)
            }
        } // ItemDelegate
    }
}
