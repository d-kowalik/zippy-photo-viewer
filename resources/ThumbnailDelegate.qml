import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0


ItemDelegate {
    Component.onCompleted: {
        console.log("Name: " + model.name + " Current file: " + myModel.currentFile)
    }

    highlighted: model.name === myModel.currentFile
    width: 400
    height: 200

    BusyIndicator {
        running: thumbnail.status === Image.Loading
        anchors.centerIn: parent
    }

    Image {
        id: thumbnail

        width: parent.width

        sourceSize.width: parent.width
        sourceSize.height: parent.height

        fillMode: Image.PreserveAspectCrop
        source: imageProvider + model.source
        asynchronous: true
        cache: true
    }

    ColorOverlay {
        width: (underline.visible) ? thumbnail.width : 0
        height: (underline.visible) ? thumbnail.height : 0
        color: "white"
        opacity: .2
    }

    Rectangle {
        id: underline
        width: parent.width
        height: 30
        color: Material.primary
        anchors.verticalCenter: parent.verticalCenter
        visible: (model.name === myModel.currentFile)

        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20
            text: model.name
            color: "white"
            elide: Text.ElideRight
        }
    }

    DropShadow {
        source: underline
        anchors.fill: underline
        visible: underline.visible
        radius: 8
        samples: 17
        color: "#80000000"
    }



    onClicked: {
        myModel.currentImageIndex = index
        myModel.currentFile = model.name
    }
}
