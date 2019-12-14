import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0

ItemDelegate {
    id: control

    property var modelIndex: model["_TreeView_ModelIndex"]
    property bool expanded: model["_TreeView_ItemExpanded"]
    property bool hasChildren: model["_TreeView_HasChildren"]
    property int depth: model["_TreeView_ItemDepth"]
    property int indent: model["_TreeView_ItemDepth"] * 16 + spacing

    signal branchIconClicked()
    signal addClicked()
    signal editClicked()
    signal removeClicked()
    signal editAccepted(string text)

    width: parent.width
    implicitWidth: 32
    hoverEnabled: true

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 4
        anchors.leftMargin: indent

        Image {
            id: branchIcon
            source: "qrc:/resources/arrow.png"
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            visible: control.hasChildren
            rotation: control.expanded ? 90 : 0
            Behavior on rotation {
                NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
            }
            MouseArea {
                id: branchIconArea
                anchors.fill: parent
                hoverEnabled: true
                onEntered: branchIcon.scale = 1.2
                onExited: branchIcon.scale = 1
                onClicked: branchIconClicked()
            }
        } // branch icon

        TextField {
            id: textField
            property bool editing: false
            text: control.text
            Layout.fillWidth: true
            Layout.fillHeight: true
            focus: editing
            readOnly: !editing
            selectByMouse: true
            background: Rectangle { color: "transparent" }
            onEditingFinished: {
                editing = false
                editAccepted(text)
            }
            onAccepted: {
                editing = false
                editAccepted(text)
            }
            onPressed: {
                event.accepted = !readOnly
            }
            onEditingChanged: {
                // bug : can not select all?
                if(editing)
                    selectAll()
            }
        }

        Item {
            id: actionsItem
            visible: children.length > 0
            implicitWidth: children.length ? children[0].children.length * 24 : 24
            Layout.minimumWidth: 32
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Row {
                anchors.centerIn: parent
                spacing: 0
                visible: hovered

                RoundButton {
                    id: buttonAdd
                    flat: !buttonAdd.hovered
                    text: "+" // todo: change to font icon
                    width: 24
                    height: 24
                    ToolTip.text: qsTr("Add")
                    ToolTip.visible: buttonAdd.hovered
                    onClicked: addClicked()
                }
                RoundButton {
                    id: buttonEdit
                    flat: !buttonEdit.hovered
                    text: "*"
                    width: 24
                    height: 24
                    ToolTip.text: qsTr("Edit")
                    ToolTip.visible: buttonEdit.hovered
                    onClicked: {
                        editClicked()
                        textField.editing = true
                        textField.selectAll()
                    }
                }
                RoundButton {
                    id: buttonRemove
                    flat: !buttonRemove.hovered
                    text: "x"
                    width: 24
                    height: 24
                    ToolTip.text: qsTr("Remove")
                    ToolTip.visible: buttonRemove.hovered
                    onClicked: removeClicked()
                }
            }
        }
    } // contentItem

    onDoubleClicked: {
        textField.editing = true
        textField.selectAll()
    }
}
