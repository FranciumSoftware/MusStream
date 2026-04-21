import QtQuick
import QtMultimedia
import QtQuick.Controls
import QtQuick.Layouts

Page {


    RowLayout {
        id: rowPlay
        anchors.fill: parent
        anchors.leftMargin: 0
        anchors.rightMargin: 0
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
