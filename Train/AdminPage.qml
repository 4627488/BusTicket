import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material

Item {
    //Material.theme: Material.Dark
    //Material.accent: Material.Purple

    Column {
        anchors.centerIn: parent
        spacing: 20 // 调整控件之间的间距

        TextField {
            id: usernameField
            placeholderText: "用户名"
        }

        TextField {
            id: passwordField
            placeholderText: "密码"
            echoMode: TextInput.Password
        }

        Button {
            text: "登录"
            //font.pixelSize: 16 // 调整按钮字体大小
            onClicked: {
                if (backend.authenticate(usernameField.text, passwordField.text)) {
                    // 进入管理员控制面板窗口，并销毁当前窗口
                    var component = Qt.createComponent("AdminControlPanel.qml");
                    if (component.status === Component.Ready) {
                        var newWindow = component.createObject(parent);
                        newWindow.show();
                    } else {
                        console.log("Error loading window: " + component.errorString());
                    }
                    mainWindow.visible = false;
                } else {
                    // 身份验证失败，显示错误消息并清空密码输入框
                    passwordField.text = ""
                    errorMessage.visible = true
                }
            }
        }

        Label {
            id: errorMessage
            text: "用户名或密码错误"
            color: "red"
            visible: false
        }

        Button {
            text: "返回主页面"
            onClicked: stackView.pop()
        }
    }
}
