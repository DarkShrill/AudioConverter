import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3

import it.videx.audio_decoder 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Audio Converter")

    color:"#f7f7f7"

    FontLoader {
        id: fontLoaderGothamMedium
        source: "qrc:/other/fonts/GothamMedium.ttf"
    }

    FontLoader {
        id: fontLoaderGothamBook
        source: "qrc:/other/fonts/GothamBook.ttf"
    }

    FontLoader {
        id: fontLoaderGothamLight
        source: "qrc:/other/fonts/GothamLight.ttf"
    }


    Button{
        id:loadFileButton

        width: 580

        text: "Load files"

        anchors.horizontalCenter: parent.horizontalCenter

        anchors.top: parent.top
        anchors.topMargin: 8

        font.family: fontLoaderGothamBook.name
        font.pointSize: 14

        onReleased: {
            folder.visible = true;
        }

    }

    Rectangle{

        id: list
        width: parent.width - 20
        height: parent.height - loadFileButton.height - 128

        color:"#e9e9e9"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: loadFileButton.bottom
        anchors.topMargin: 16

        border.color: "#d2d2d2"
        border.width: 1

        ListView{

            property var files : []
            id:listView

            anchors.fill: parent
            spacing: 10
            orientation: Qt.Vertical
            model : files

            snapMode: ListView.SnapToItem

            highlightMoveDuration : 15000//20000
            maximumFlickVelocity: 7500//10000
            clip: true

            delegate: Rectangle{

                color: "transparent"

                anchors.horizontalCenter: parent.horizontalCenter

                width: parent.width - 4
                height: 40

                Text{
                    font.family: fontLoaderGothamBook.name
                    font.pointSize: 10
                    anchors.centerIn: parent
                    text: modelData
                }
            }

            FileDialog {
                id:folder

                selectMultiple: true
                nameFilters: ["Audio Files (*.wav)"]
                visible: false
                onAccepted: {
                    var files = [];

                    for(var i = 0; i < fileUrls.length; i++){
                        files.push(fileUrls[i].replace(/^file:\/{3}/,""))
                    }
                    listView.files = files;
                }
            }
        }
    }

    Connections{
        target: AudioFilesDecoder

        onFilesDecoded:{
            console.log("FILES DECODED");
        }
    }


    Grid{
        anchors.top: list.bottom
        anchors.topMargin: 32

        anchors.horizontalCenter: parent.horizontalCenter

        columns: 3
        spacing: 10

        Button{
            id:startConversion

            width: 580/2.5

            text: "Convert Files"
            font.family: fontLoaderGothamBook.name
            font.pointSize: 14

            onReleased: {
                if(listView.model.length > 0){
                    AudioFilesDecoder.decode(listView.model, sampleRate.currentText.replace(" Hz", ""), generateAlsoWavFile.checked)
                }
            }
        }

        ComboBox{
            id: sampleRate

            width: 580/4

            height: startConversion.height

            currentIndex: 1

            model: ["8000 Hz", "16000 Hz", "44100 Hz"]



            contentItem: Text {
                text: parent.displayText
                font.family: fontLoaderGothamBook.name
                font.pointSize: 10
                verticalAlignment: Text.AlignVCenter;
                horizontalAlignment: Text.AlignHCenter;
                elide: Text.ElideRight
            }

            delegate: ItemDelegate {
                width: parent.width
                text: parent.textRole ? (Array.isArray(parent.model) ? modelData[parent.textRole] : model[parent.textRole]) : modelData
                font.family: fontLoaderGothamBook.name
                font.pointSize: 10
                highlighted: parent.highlightedIndex === index
                hoverEnabled: parent.hoverEnabled
            }
        }

        CheckBox{

            id: generateAlsoWavFile
            text: "Generate also wav file"
            font.family: fontLoaderGothamBook.name
            font.pointSize: 12


        }
    }
}
