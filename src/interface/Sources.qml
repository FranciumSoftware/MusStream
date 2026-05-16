import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

Page {
    id: root
    background: Rectangle {
        color: "#dddddd"
    }
    // Déclare sourcelist comme une propriété
    property var sourcelist: []

    MessageDialog {
        id: messageDialog
        title: "Success"
        text: qsTr("Unable to initialize the database")
        buttons: MessageDialog.Ok
    }

    Component.onCompleted: {
        sourcelist = cppManager.getSources(); // Charge les sources au démarrage
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        Text {
            text: qsTr("Add sources")
            font.pointSize: 20
            Layout.margins: 0
        }
        RowLayout {
            TextField {
                id: urlField
                Layout.fillWidth: true
                Layout.margins: 0
                color: "black"
                placeholderTextColor: "#aaaaaa"
                placeholderText: qsTr("Enter the source url here. Don't forget the https://")
                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 40
                    color: urlField.enabled ? "#ededed" : "#353637"
                    radius: 7
                }
            }
            Button {
                id: addSource
                text: qsTr("Add source")
                flat: true
                palette {
                    buttonText: "black"
                    brightText: "black"
                    highlight: "black"
                    windowText: "white"
                }

                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    color: addSource.down ? "#17d10a" : "#05a12d"
                    radius: 7
                }
                onClicked: {
                    cppManager.addSource(urlField.text);
                }
                Connections {
                    target: cppManager
                    function onSourceAddedSuccess() {
                        messageDialog.text = "Source ajoutée avec succès !";
                        messageDialog.open();
                        sourcelist = cppManager.getSources(); // Rafraîchit la liste après ajout
                    }
                    function onSourceAddedError(message) {
                        messageDialog.text = "Erreur : " + message;
                        messageDialog.open();
                    }
                }
            }
        }
        ColumnLayout {
            Layout.fillHeight: true
            Text {
                text: qsTr("Manage sources")
                font.pointSize: 20
            }

            ListView {
                Layout.fillHeight: true
                model: sourcelist
                delegate: RowLayout {
                    Text {
                        text: modelData.name
                        font.pointSize: 15
                    }
                    Button {
                        text: qsTr("Delete Source")
                        onClicked: {
                            var success = cppManager.deleteSource(modelData.ID);
                            if (!success) {
                                messageDialog.text = "Impossible to delete source. Check logs for more details";
                                messageDialog.open();
                            } else {
                                sourcelist = cppManager.getSources(); // Rafraîchit la liste après suppression
                            }
                        }
                    }
                }
            }
        }
    }
}
