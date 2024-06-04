import QtQuick
import TableModel
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 650
    height: 600
    title: "管理员面板"
    Material.theme: Material.Light
    Material.accent: Material.Green

    property string filterText: ""
    property int filterOption: 0

    TableView {
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        id: tableView

        model: TableModel {
            id: tableModel
        }

        delegate: Rectangle {
            implicitWidth: 80
            implicitHeight: 30
            Text {
                text: display
                anchors.centerIn: parent
                font.pixelSize: 15
            }
        }
    }
    
    Dialog {
        id: addDialog
        title: "添加新的车次"
        visible: false
        standardButtons: Dialog.Ok | Dialog.Cancel

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Column {
            spacing: 10
            //"车次号", "发车时间", "始发站", "终到站", "大约时长", "票价", "最大载客量","已售票数"
            GridLayout {
                columns: 2
                //spacing: 10

                TextField {
                    id: trainNumberField
                    placeholderText: "车次号"
                    validator: IntValidator {} //只允许填数字
                }
                TextField {
                    id: departureTimeField
                    placeholderText: "发车时间"
                }
                TextField {
                    id: startStationField
                    placeholderText: "始发站"
                }
                TextField {
                    id: endStationField
                    placeholderText: "终到站"
                }
                TextField {
                    id: durationField
                    placeholderText: "大约时长"
                    validator: IntValidator {} //只允许填数字
                }
                TextField {
                    id: priceField
                    placeholderText: "票价"
                    validator: IntValidator {} //只允许填数字
                }
                TextField {
                    id: capacityField
                    placeholderText: "最大载客量"
                    validator: IntValidator {} //只允许填数字
                }
                TextField {
                    id: soldTicketsField
                    placeholderText: "已售票数"
                    validator: IntValidator {} //只允许填数字
                }
            }
        }

        onAccepted: {
            backend.add(trainNumberField.text, departureTimeField.text ,startStationField.text, endStationField.text, durationField.text, priceField.text, capacityField.text, soldTicketsField.text)
            trainNumberField.text = ""
            departureTimeField.text = ""
			startStationField.text = ""
			endStationField.text = ""
			durationField.text = ""
			priceField.text = ""
			capacityField.text = ""
			soldTicketsField.text = ""
            tableModel.updateModel() //刷新表格
        }
    }
    
    Dialog {
        id: notificationDialog
        title: "通知"
        Label {
            text: "s"
        }
        visible: false
    }

    Dialog {
        id: delDialog
        title: "删除指定车次"
        visible: false
        standardButtons: Dialog.Ok | Dialog.Cancel

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Column {
            spacing: 10

            GridLayout {
                columns: 2

                TextField {
                    id: trainNumberField2
                    placeholderText: "车次号"
                }
            }

        }
        
        onAccepted: {
            var result = backend.removeBusInfo(trainNumberField2.text)
            trainNumberField2.text = ""
            tableModel.updateModel() //刷新表格
            notificationDialog.text = result
            notificationDialog.open()
        }
    }

    RowLayout {
        id: bottomLayout
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10
    
        CheckBox {
            id: checkBox
            text: "筛选"
            onClicked: {
                tableModel.setFilter(checkBox.checked, filterOption, filterText);
            }
        }
        TextField {
            id: filterTextField
            Layout.fillWidth: true
            placeholderText: "筛选"
            onTextChanged: {
                filterText = text;
                tableModel.setFilter(checkBox.checked, filterOption, filterText);
            }
        }

        ComboBox {
            id: comboBox
            width: 200
            model: ["车次号", "始发站", "终到站"]

            onActivated: {
                filterOption = currentIndex;
                tableModel.setFilter(checkBox.checked, filterOption, filterText);
            }
        }

        Item {
            Layout.fillWidth: true
        }

        Label {
            // 表头不算结果
            id: pageNumLabel
            text: "共 " + (tableModel.rowCount() - 1) + " 次列车"
        }
    
        Item {
                Layout.fillWidth: true
        }

        Button {
		    text: "添加"
		    onClicked: {
			    addDialog.open()
		    }
	    }

        Button {
		    text: "删除"
		    onClicked: {
			    delDialog.open()
		    }
	    }
    }
}