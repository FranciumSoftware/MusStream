import QtQuick
import QtMultimedia
import QtQuick.Controls
import QtQuick.Layouts

Page {
    background: Rectangle {
        color: "#dddddd"
    }
    QtObject {
        id: globalSearch
    }
    signal navigateTo(string page)
    ColumnLayout {
        width: parent.width;
        Rectangle {
            color: "#ececec"
            Layout.margins: 10;
            Layout.fillWidth: true;
            radius: 15;
            height: 75;
            width: parent.width - 20;
            RowLayout {
                anchors.fill: parent
                anchors.rightMargin: 30;
                anchors.leftMargin: 30;
                spacing: 20;
                Text {
                    text: "Welcome Back !"
                    fontSizeMode: Text.Fit
                    font.pointSize: 15;
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.rightMargin: 20
                }
                RowLayout {
                    spacing: 8
                    Layout.alignment: Qt.AlignRight
                    Item {
                        width: 100;
                        height: 40;

                        signal goToSearchPage()
                        Rectangle {
                            anchors.fill: parent;
                            color: "#17d10a";
                            radius: 10;
                        }
                        MouseArea {
                            id: searchFromHome
                            anchors.fill: parent;
                            onClicked: navigateTo("interface/Search.qml")
                            cursorShape: Qt.PointingHandCursor
                        }
                        Text {
                            text: qsTr("Search")
                            font.pixelSize: 10
                            anchors.centerIn: parent
                        }
                    }
                    Item {
                        width: 100;
                        height: 40;

                        signal goToSearchPage()
                        Rectangle {
                            anchors.fill: parent;
                            color: "#17d10a";
                            radius: 10;
                        }
                        MouseArea {
                            id: sourcesFromHome
                            anchors.fill: parent;
                            onClicked: navigateTo("interface/Sources.qml")
                            cursorShape: Qt.PointingHandCursor
                        }
                        Text {
                            text: qsTr("Manage sources")
                            font.pixelSize: 10
                            anchors.centerIn: parent
                        }
                    }
                }


            }


        }
    }
}
