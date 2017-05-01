import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Item {
    anchors.fill: parent

    Component.onCompleted: {
        window.header = toolbar
    }

    MessageDialog {
        id: fileErrorDialog

        title: "Error"
        text:  "Failed to open the file"
    }

    MessageDialog {
        id: passwordError

        title: "Error"
        text:  "Password not correct"
    }

    Dialog {
        id: passwordDialog

        title: "Password"
        standardButtons: Dialog.Ok | Dialog.Cancel

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
        }

        onButtonClicked: {
            if (clickedButton === Dialog.Ok) {
                archive.password = passwordField.text
            }
        }
    }

    Connections {
        target: archive
        onFailedToOpen: {
            fileErrorDialog.open()
        }

        onPasswordRequired: {
            passwordDialog.open()
        }

        onPasswordCorrect: {
            archive.path = zipFilePath.text
        }

        onPasswordIncorrect: {
            passwordError.open()
        }
    }

    ToolBar {
        id: toolbar
        Layout.fillWidth: parent

        RowLayout {
            ToolButton {
                height: 48
                width: 48
                onClicked: mainStackView.pop()

                contentItem: Image {
                    source: "images/back_arrow.png"
                }
            }

            Label {
                text: qsTr("Settings")
                id: titleLabel
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }
    }


    Column {
        padding: 5
        width: parent.width/2 - padding

        Label {
            text: qsTr("Zip file path")
            font.pixelSize: 20
            elide: Label.ElideLeft
            width: parent.width
        }

        Column {
            width: parent.width - padding
            padding: 5

            Row {
                id: inputRow
                width: parent.width
                bottomPadding: 2
                topPadding: 3

                TextField {
                    id: zipFilePath
                    placeholderText: qsTr("path")
                    width: parent.width
                    text: archive.path
                }

                ToolButton {
                    height: zipFilePath.height
                    width: height
                    id: browseButton
                    text: qsTr("...")
                    onClicked: zipFilePathDialog.open()
                }
            }

            Button {
                anchors.left: inputRow.left
                text: qsTr("apply")
                onClicked: {
                    archive.path = zipFilePath.text
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
}
