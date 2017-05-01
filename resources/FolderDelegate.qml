import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0


ItemDelegate {
    width: 400
    height: 65

    Component.onCompleted: {
        console.log("Folder name: " + model.source)
    }

    Row {
        spacing: 20
        x: parent.width / 10
        anchors.verticalCenter: parent.verticalCenter

        Image {
            id: thumbnailImage
            source: "images/folder.png"
        }

        Label {
            id: folderName
            font.pixelSize: 15
            anchors.verticalCenter: thumbnailImage.verticalCenter

            text: model.source
        }
    }


    onClicked: {
        myModel.currentFolderName = model.source
    }
}
