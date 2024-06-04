import QtQuick
import TableModel
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: "管理员车票窗口"
    Material.theme: Material.Light
    Material.accent: Material.Green


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
                //onEditingFinished: tableModel.update(index, text) // 当编辑完成时，更新模型
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
                }
                TextField {
                    id: departureTimeField
                    placeholderText: "发车时间（冒号分割）"
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
                }
                TextField {
                    id: priceField
                    placeholderText: "票价"
                }
                TextField {
                    id: capacityField
                    placeholderText: "最大载客量"
                }
                TextField {
                    id: soldTicketsField
                    placeholderText: "已售票数"
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
            backend.removeBusInfo(trainNumberField2.text)
			trainNumberField2.text = ""
            tableModel.updateModel() //刷新表格
        }
    }


    Button {
		text: "添加"
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		onClicked: {
			addDialog.open()
		}
	}
    Button {
		text: "删除"
		anchors.bottom: parent.bottom
		//anchors.right: add.left
		onClicked: {
			delDialog.open()
		}
	}
}