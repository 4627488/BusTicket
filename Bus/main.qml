import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


ApplicationWindow {
    id: mainWindow
    visible: true
    width: 300
    height: 400
    title: "车票管理系统"
    Material.theme: Material.Light
    Material.accent: Material.Purple 


    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Component {
            Item {
                Column {
                    anchors.centerIn: parent
                    spacing: 20
                    Label {
                        id: titleLabel
                        text: "选择身份"
                        //color: "red"
						//font.pixelSize: 20
						horizontalAlignment: Text.AlignHCenter
                        visible: true
                    }

                    Button {
                        text: "进入管理员窗口"
                        onClicked: stackView.push("AdminPage.qml")
                    }

                    Button {
                        text: "进入普通顾客窗口"
                        onClicked: {
							var component = Qt.createComponent("ControlPanel.qml");
                            if (component.status === Component.Ready) {
                                var newWindow = component.createObject(parent);
                                newWindow.isAdmin = false; 
                                newWindow.show();
                            } else {
                                console.log("Error loading window: " + component.errorString());
                            }
                            mainWindow.visible = false;
						}
                    }

                    Button {
                        text: "退出系统"
                        onClicked: Qt.quit()
                    }
                }
            }
        }
    }
}