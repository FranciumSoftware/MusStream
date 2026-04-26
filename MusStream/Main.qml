import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Dialogs

ApplicationWindow {
    id: window
    visible: true
    width: 900
    height: 600
    title: "MusStream"
    background: Rectangle {
        color: "#dedede"
    }

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
            color: "#ededed" // Couleur de fond de la barre
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
                    color: "black"
                }
                Text {
                    id: artist
                    text: "Artiste"
                    color: "black"
                }
            }

            Button {
                icon.source: globalMediaPlayer.playbackState === MediaPlayer.PlayingState ? "qrc:/icons/Assets/pause.svg" : "qrc:/icons/Assets/play.svg"
                icon.color: "black"
                onClicked: {
                    if (globalMediaPlayer.playbackState === MediaPlayer.PlayingState) {
                        globalMediaPlayer.pause();
                    } else {
                        globalMediaPlayer.play();
                    }
                }
                background: Rectangle {
                    color: "transparent"
                }
                palette: {
                    buttonText: "black"
                    brightText: "black"
                    highlight: "black"
                    windowText: "black"
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
                background: Rectangle {
                    x: timeMusic.leftPadding
                            y: timeMusic.topPadding + timeMusic.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: timeMusic.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#bdbebf"

                            Rectangle {
                                width: timeMusic.visualPosition * parent.width
                                height: parent.height
                                color: "#21be2b"
                                radius: 2
                            }
                    }
                handle: Rectangle {
                    x: timeMusic.leftPadding + timeMusic.visualPosition * (timeMusic.availableWidth - width)
                            y: timeMusic.topPadding + timeMusic.availableHeight / 2 - height / 2
                            implicitWidth: 13
                            implicitHeight: 13
                            radius: 6.5
                            color: timeMusic.pressed ? "#f0f0f0" : "#f6f6f6"
                            border.color: "#bdbebf"
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
            Layout.preferredWidth: isMenuOpen ? 200 : 70
            Behavior on Layout.preferredWidth {
                NumberAnimation {
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }

            color: "#ededed"

            ColumnLayout {
                anchors.fill: parent
                spacing: 5

                // Bouton de bascule (Burger / Arrow)
                Button {
                    id: menuButton
                    text: isMenuOpen ? "<font color='#010101'>\ue81d</font>" : "<font color='#010101'>\uf0c9</font>"
                    font.family: iconFont.name
                    flat: true
                    display: AbstractButton.TextOnly
                    font.pixelSize: 20
                    palette.buttonText: "#000000"
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 20
                    onClicked: isMenuOpen = !isMenuOpen
                    background: Rectangle {
                        color: "transparent"
                    }
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
                                color: parent.down ? "#black" : "black"
                                horizontalAlignment: Text.AlignHCenter
                                Layout.preferredWidth: 50
                            }
                            Text {
                                text: model.name
                                color: "black"
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
                            color: parent.hovered ? "#dddddd" : "transparent"
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
