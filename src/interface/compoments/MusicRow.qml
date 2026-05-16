import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    width: parent.width
    height: 250
    color: "transparent"

    // --- PROPRIÉTÉS PERSONNALISABLES ---
    property alias title: titleLabel.text
    property alias musicModel: horizontalList.model

    // --- SIGNAL ---
    // On définit un signal que le parent pourra écouter
    signal musicClicked(string title, string artist, string url, string url_cover)

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            id: titleLabel
            text: "Catégorie"
            color: "black"
            font.pixelSize: 22
            font.bold: true
            leftPadding: 10
        }

        ListView {
            id: horizontalList
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: ListView.Horizontal
            spacing: 15
            clip: true
            leftMargin: 10
            rightMargin: 10

            delegate: Rectangle {
                width: 150
                height: 200
                color: "#fefefe"
                radius: 10

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    // Affichage de la Cover
                    Rectangle {
                        Layout.preferredWidth: 130
                        Layout.preferredHeight: 130
                        color: "#9e9e9e"
                        radius: 6
                        clip: true

                        Image {
                            anchors.fill: parent
                            source: modelData.url_cover // Utilise la propriété 'cover' du modèle
                            fillMode: Image.PreserveAspectCrop
                            // Image par défaut si la source est vide
                            visible: status === Image.Ready
                        }

                        /*Text {
                            anchors.centerIn: parent
                            text: "🎵"
                            font.pixelSize: 40
                            visible: parent.children[0].status !== Image.Ready
                        }*/
                    }

                    Text {
                        text: modelData.name // Propriété 'name'
                        color: "black"
                        font.bold: true
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    Text {
                        text: modelData.artist // Propriété 'artist'
                        color: "#191919"
                        font.pixelSize: 12
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = "#bebebe"
                    onExited: parent.color = "#fefefe"

                    // --- DÉCLENCHEMENT DU SIGNAL ---
                    onClicked: {
                        root.musicClicked(modelData.name, modelData.artist, modelData.url, modelData.url_cover)
                    }
                }
            }
        }
    }
}