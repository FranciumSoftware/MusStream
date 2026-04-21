import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Dialogs

ApplicationWindow {
    id: window
    visible: true
    width: 900
    height: 600
    title: "MusStream"

    signal changeMediaSource(string source)
    Component.onCompleted: {
        initiateDB()
        globalMediaPlayer.sourceChanged.connect(function(newSource) {
            title.text = cppManager.getName();
            artist.text = cppManager.getArtist();
            cover.source = cppManager.getCover();

        })
    }

    MessageDialog {
        id: errorDialogDatabase
        title: "Erreur Base de Données"
        text: qsTr("Impossible d'initialialiser la base de données")
        buttons: MessageDialog.Ok
    }

    function initiateDB() {
        var code = cppManager.init();

        if (code === 1) {
            // 2. Ouvrir le dialogue si besoin
            errorDialogDatabase.open();
        } else {
            console.log("Success of initialization");
        }
    }



    FontLoader {
        id: iconFont
        source: "qrc:/font/Assets/mustream.ttf"
    }
    header: ToolBar {
        background: Rectangle {
            color: "#05a12d" // Couleur de fond de la barre
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
                    Layout.preferredHeight: 40
                    Layout.maximumWidth: 60
                    source: "qrc:/images/Assets/default_cover.png"

            }



            ColumnLayout {
                Layout.preferredWidth: 150
                Text {
                    id: title
                    text: "Titre"
                    color: "white"
                }
                Text {
                    id: artist
                    text: "Artiste"
                    color: "white"
                }
            }

            Button {
                icon.source: globalMediaPlayer.playbackState === MediaPlayer.PlayingState ? "qrc:/icons/Assets/pause.svg" : "qrc:/icons/Assets/play.svg"
                onClicked: {
                    if (globalMediaPlayer.playbackState === MediaPlayer.PlayingState) {
                        globalMediaPlayer.pause();
                    } else {
                        globalMediaPlayer.play();
                    }
                }
            }

            Slider {
                id: timeMusic
                to: globalMediaPlayer.duration
                value: globalMediaPlayer.position
                anchors.leftMargin: 0
                anchors.rightMargin: 0

                Layout.fillWidth: true
                Layout.preferredWidth: 80
                onValueChanged: {
                    if (timeMusic.pressed) {
                        globalMediaPlayer.position = timeMusic.value;
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

            color: "#05a12d"

            ColumnLayout {
                anchors.fill: parent
                spacing: 5

                // Bouton de bascule (Burger / Arrow)
                Button {
                    text: isMenuOpen ? "<font color='#fefefe'>" + "\ue81d" + "</font>" : "<font color='#fefefe'>\uf0c9</font>"
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
                            name: "Home"
                            iconCode: "\ue800"
                            page: "interface/Home.qml"
                        }
                        ListElement {
                            name: "Search"
                            iconCode: "\ue806"
                            page: "interface/Search.qml"
                        }
                        ListElement {
                            name: "Sources"
                            iconCode: "\ue811"
                            page: "interface/Sources.qml"
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
        // Exemple d'appel au démarrage



        // --- ZONE DE CONTENU ---
        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: "interface/Home.qml"
        }
    }
}
