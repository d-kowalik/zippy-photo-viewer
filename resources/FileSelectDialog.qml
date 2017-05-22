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

    onClosed: {
        fileErrorText.visible = false
    }

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
                text: archive.path
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

        RowLayout {

            ToolButton {
                anchors.horizontalCenter: Qt.AlignRight
                text: "Ok"
                onClicked: {
                    archive.path = zipFilePath.text
                }
            }

            Label {
                id: fileErrorText
                color: Material.color("red")
                text: "Failed to open the file"
                visible: false
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

    Popup {
        id: passwordDialog

        x: (mainWindow.width - width) / 2
        y: (mainWindow.height - height) / 2

        parent: ApplicationWindow.overlay

        ColumnLayout {
            spacing: 20
            anchors.fill: parent
            Label {
                elide: Label.ElideRight
                text: "Please enter the password: "
                Layout.fillWidth: true
            }

            TextField {
                id: passwordField
                placeholderText: "Password"
                echoMode: TextField.Password
                Layout.fillWidth: true
            }

            Label {
                id: passwordErrorText
                elide: Label.ElideRight
                color: Material.color("red")
                text: "Incorrect password"
                visible: false
            }

            RowLayout {
                ToolButton {
                    text: "Apply"
                    onClicked: {
                        archive.password = passwordField.text
                    }
                }
            }
        }

        onClosed: {
            passwordErrorText.visible = false
            passwordField.text = ""
        }
    }

    Connections {
        target: archive
        onFailedToOpen: {
            fileErrorText.visible = true
        }

        onPathChanged: {
            fileSelectDialog.close()
        }

        onPasswordRequired: {
            passwordDialog.open()
        }

        onPasswordCorrect: {
            archive.path = zipFilePath.text
            passwordDialog.close()
        }

        onPasswordIncorrect: {
            passwordErrorText.visible = true
        }
    }
}
