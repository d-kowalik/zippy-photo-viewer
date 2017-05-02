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
        height: 20
        color: Material.primary
        anchors.bottom: parent.bottom
        visible: (model.name === myModel.currentFile)

        Label {
            width: parent.width
            height: parent.height
            text: model.name
            color: "white"
            elide: Text.ElideRight
        }
    }

    Rectangle {
        color: underline.color
        width: parent.width
        height: 1
        anchors.top: parent.top
        visible: underline.visible
    }

    Rectangle {
        color: underline.color
        width: 1
        height: parent.height
        anchors.left: parent.left
        visible: underline.visible
    }

    Rectangle {
        color: underline.color
        width: 1
        height: parent.height
        anchors.right: parent.right
        visible: underline.visible
    }


    onClicked: {
        myModel.currentImageIndex = index
        myModel.currentFile = model.name
    }
}
