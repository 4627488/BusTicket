import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    Column {
        anchors.centerIn: parent
        spacing: 20

        Label {
            text: "欢迎来到顾客页面"
        }

        Button {
            text: "返回主页面"
            onClicked: stackView.pop()
        }

        // 在这里添加顾客的功能
    }
}
