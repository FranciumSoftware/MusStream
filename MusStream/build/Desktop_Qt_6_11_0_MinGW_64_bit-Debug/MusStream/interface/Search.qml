import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

Page {
    id: root

    // Déclarez une propriété pour les résultats de recherche
    property var searchResults: []

    // Connexion pour rafraîchir les résultats de recherche
    Connections {
        target: cppManager
        function onSearchResultsChanged() {
            searchResults = cppManager.searchResults; // Met à jour la liste des résultats
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 20
        anchors.leftMargin: 20

        Text {
            text: qsTr("Search Music")
            font.pointSize: 20
            Layout.margins: 0
        }

        RowLayout {
            TextField {
                id: searchBox
                Layout.fillWidth: true
                placeholderText: qsTr("Search music by title")
            }

            Button {
                id: searchButton
                text: qsTr("Search")
                flat: true
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    color: searchButton.down ? "#16db0b" : "#b8f909"
                }
                onClicked: {
                    cppManager.searchMusic(searchBox.text);
                }
            }
        }

        ListView {
            Layout.fillHeight: true
            model: searchResults
            delegate: RowLayout {
                Rectangle {
                    width: 50
                    height: 50
                    clip: true // Empêche l'image de dépasser

                    Image {
                        source: modelData.url_cover
                        width: 20
                        height: 20
                        fillMode: Image.PreserveAspectFit
                        anchors.fill: parent
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            cppManager.setInfosTo(modelData.name, modelData.artist, modelData.url_cover);
                            updateSourceTimer.start()
                        }
                    }
                }

                Text {
                    text: modelData.name
                    font.pointSize: 15
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            cppManager.setInfosTo(modelData.name, modelData.artist, modelData.url_cover);
                            updateSourceTimer.start()
                        }
                    }
                }
                Text {
                    text: modelData.artist
                    font.pointSize: 15
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            cppManager.setInfosTo(modelData.name, modelData.artist, modelData.url_cover);
                            updateSourceTimer.start()
                        }
                    }
                    Timer {
                        id: updateSourceTimer
                        interval: 200  // Retard de 100 ms
                        repeat: false
                        onTriggered: {
                            globalMediaPlayer.source = modelData.url
                            globalMediaPlayer.play()
                        }
                    }
                }
                MouseArea {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onClicked: {
                        globalMediaPlayer.source = modelData.url
                        globalMediaPlayer.play()
                        cppManager.setInfosTo(modelData.name, modelData.artist, modelData.url_cover);
                    }
                }
            }
        }
    }
}
