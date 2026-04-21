import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

ApplicationWindow {
    id: window
    visible: true
    width: 900
    height: 600
    title: "MusStream"
    FontLoader {
        id: iconFont
        source: "qrc:/font/Assets/mustream.ttf" // Chemin vers votre fichier
    }
    MediaPlayer {
        id: playMusic
        source: "https://www.soundhelix.com/examples/mp3/SoundHelix-Song-8.mp3"
        audioOutput: AudioOutput {}
    }
    header: ToolBar {
        background: Rectangle {
            color: "#01731f" // Couleur de fond de la barre
            implicitHeight: 50
        }
        RowLayout {
            id: rowPlay
            anchors.fill: parent
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            Image {
                id: cover
                fillMode: Image.PreserveAspectFit
                height: 5
                width: 5
                source: "qrc:/images/Assets/default_cover.png"

            }

            ColumnLayout {
                Layout.preferredWidth: 150
                Text {
                    text: "Titre"
                    id: title
                    color:  "white"
                }
                Text {
                    text: "Artiste"
                    id: artist
                    color: "white"
                }
            }

            Button {
                icon.source: playMusic.playbackState === MediaPlayer.PlayingState ? "qrc:/icons/Assets/pause.svg" : "qrc:/icons/Assets/play.svg"
                onClicked: {
                    if (playMusic.playbackState === MediaPlayer.PlayingState) {
                        playMusic.pause()
                    } else {
                        playMusic.play()
                    }
                }
            }

            Slider {
                id: timeMusic
                to: playMusic.duration
                value: playMusic.position
                anchors.leftMargin: 0
                anchors.rightMargin: 0

                Layout.fillWidth: true
                Layout.preferredWidth: 80
                onValueChanged: {
                    if (timeMusic.pressed) {
                        playMusic.position = timeMusic.value
                    }
                }
            }
        }
    }
    // État du menu : vrai = large, faux = réduit
    property bool isMenuOpen: true

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // --- BARRE LATÉRALE ---
        Rectangle {
            id: sideBar
            Layout.fillHeight: true

            // Animation fluide de la largeur
            Layout.preferredWidth: isMenuOpen ? 200 : 60
            Behavior on Layout.preferredWidth {
                NumberAnimation {
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }

            color: "#01731f"

            ColumnLayout {
                anchors.fill: parent
                spacing: 5

                // Bouton de bascule (Burger / Arrow)
                Button {
                    text: isMenuOpen ? "<font color='#fefefe'>" + "\ue81d"
                                       + "</font>" : "<font color='#fefefe'>\uf0c9</font>"
                    font.family: iconFont.name
                    flat: true
                    display: AbstractButton.TextOnly
                    font.pixelSize: 20
                    palette.buttonText: "#ff00ff"
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 20
                    onClicked: isMenuOpen = !isMenuOpen
                }

                ListView {
                    id: menuList
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    clip: true // Évite que le texte dépasse pendant l'animation
                    model: ListModel {
                        ListElement {
                            name: "Accueil"
                            iconCode: "\ue800"
                            page: "Home.qml"
                        }
                        ListElement {
                            name: "Rechercher"
                            iconCode: "\ue806"
                            page: "Search.qml"
                        }
                    }

                    delegate: ItemDelegate {
                        width: sideBar.width
                        height: 50

                        contentItem: RowLayout {
                            spacing: 0
                            // L'icône sous forme de texte
                            Text {
                                text: model.iconCode // Ex: "\uf015"
                                font.family: iconFont.name // On utilise le nom chargé
                                font.pixelSize: 20
                                color: parent.down ? "#1abc9c" : "white"
                                horizontalAlignment: Text.AlignHCenter
                                Layout.preferredWidth: 50
                            }
                            Text {
                                text: model.name
                                color: "white"
                                font.pixelSize: 14
                                Layout.fillWidth: true
                                // On cache le texte si le menu est fermé
                                opacity: isMenuOpen ? 1.0 : 0.0
                                Behavior on opacity {
                                    NumberAnimation {
                                        duration: 150
                                    }
                                }
                            }
                        }

                        background: Rectangle {
                            color: parent.hovered ? "#07a631" : "transparent"
                        }

                        onClicked: stackView.replace(model.page)
                    }
                }
            }
        }

        // --- ZONE DE CONTENU ---
        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: "Home.qml"
        }
    }
}
