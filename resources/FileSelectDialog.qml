import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import QtQuick.Window 2.0

Popup {
    id: fileSelectDialog

    width: mainWindow.width / 2

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 2

    focus: true
    modal: true

    parent: ApplicationWindow.overlay

    ColumnLayout {
        spacing: 20
        anchors.fill: parent
        width: parent.width
        Layout.fillWidth: true

        Label {
            elide: Label.ElideRight
            text: "Zip file path:"
            Layout.fillWidth: true
            font.pixelSize: 25
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            TextField {
                id: zipFilePath
                Layout.fillWidth: true
                focus: true
                placeholderText: "path"
                font.pixelSize: 20
            }
            Button {
                id: fileBrowseButton
                text: "..."
                highlighted: true
                onClicked: {
                    zipFilePathDialog.open()
                }
            }
        }

        ToolButton {
            anchors.horizontalCenter: Qt.AlignRight
            text: "Ok"
            onClicked: {
                archive.path = zipFilePath.text
                fileSelectDialog.close()
            }
        }
    }

    FileDialog {
        id: zipFilePathDialog
        nameFilters: [ "Zip files (*.zip)" ]

        onSelectionAccepted: {
            zipFilePath.text = fileUrl.toString().split("///")[1] // It gets rid of file:///, gotta replace this line with something better
        }
    }
}
