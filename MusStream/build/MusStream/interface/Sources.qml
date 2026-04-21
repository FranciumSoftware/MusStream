import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts

Page {
    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        Text {
            text: "Ajouter une source"
            font.pointSize: 20
            Layout.margins: 0
        }
        RowLayout {
            TextField {
                id: urlField
                Layout.fillWidth: true
                Layout.margins: 0
            }
            Button {
                id: addSource
                text: qsTr("Ajouter la source")
                flat: true
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    color: addSource.down ? "#3df29d" : "#EEEE00"

                    border.color: "transparent"
                }
                onClicked: {
                    cppManager.addSource(urlField.text);
                }

            }

        }
    }
}
