import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import QtQuick.Window 2.0

Rectangle {
    id: mainContainer
    Layout.fillWidth: parent
    Layout.fillHeight: parent
    anchors.fill: parent
    color: Material.primary

    property alias stackView: mainStackView
    property alias window: mainContainer

    property string imageProvider: qsTr("image://zipimageprovider")

    function toSource(imagePath) {
        return imageProvider + imagePath
    }

    Connections {
        target: archive
        onPathChanged: {
            myModel.reloadFolderStructure()
            myModel.refresh()
        }
    }

    Rectangle {
        id: toolbar
        Layout.fillWidth: parent
        width: parent.width
        height: 30
        anchors.top: parent.top
        color: "transparent"
        z: 10

        MouseArea {
            anchors.fill: parent

            onPressed: mainWindow.toolbarLeftMouseEvent()
            onDoubleClicked: mainWindow.toolbarDoubleClicked()
        }

        Row {
            anchors.left: parent.left
            spacing: 20

            ToolButton {
                id: drawerButton
                width: 35
                height: 35
                z: 10

                contentItem: Image {
                    source: stackView.depth > 1 ? "images/back_arrow.png" : "images/hamburger.png"
                }

                onClicked: {
                    if (stackView.depth > 1)
                        stackView.pop()
                    else
                        drawer.open()
                }
            }

            Label {
                id: toolbarTitle
                text: "Zippy"
                y: 3

                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }


        Row {
            anchors.right: parent.right
            z: 10

            ToolButton {
                onClicked: mainWindow.minimize()
                width: 26
                height: 26

                contentItem: Rectangle {
                    color: "#4CAF50"
                    radius: parent.width
                }
            }

            ToolButton {
                onClicked:  mainWindow.maximize()
                width: 26
                height: 26

                contentItem: Rectangle {
                    color: "#FFC107"
                    radius: parent.width
                }
            }

            ToolButton {
                onClicked: mainWindow.close()
                width: 26
                height: 26

                contentItem: Rectangle {
                    color: "#F44336"
                    radius: parent.width
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: 448
        height: parent.height

        onClosed: mainStackView.focus = true

        Rectangle {
            anchors.fill: parent
            color: Material.background

            ListView {
                id: drawerListView

                width: parent.width - sideMenu.width
                height: parent.height
                anchors.right: parent.right
                Layout.fillHeight: parent

                delegate: Component {
                    Loader {
                        source: switch(type) {
                                case "Thumbnail": return "qrc:/ThumbnailDelegate.qml"
                                case "Folder": return "qrc:/FolderDelegate.qml"
                                }
                    }
                }

                ListModel {
                    id: drawerListModel
                }

                model: myModel

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            Rectangle {
                id: sideMenu
                color: Material.background
                width: 48
                height: parent.height
                Layout.fillHeight: parent
                anchors.left: parent.left

                Column {
                    width: parent.width
                    height: parent.height

                    ToolButton {
                        width: parent.width
                        height: parent.width

                        contentItem: Image {
                            source: myModel.currentFolderName === "/" ? "images/hamburger.png" : "images/back_arrow.png"
                            anchors.centerIn: parent
                        }

                        onClicked: {
                            if (myModel.currentFolderName === "/")
                                drawer.close()
                            else
                                myModel.currentFolderName = myModel.parentFolderName
                        }
                    }

                    ToolButton {
                        width: parent.width
                        height: parent.width

                        contentItem: Image {
                            source: "images/settings.png"
                        }

                        onClicked: {
                            drawer.close()
                            toolbarTitle.text = "Settings"
                            mainStackView.push("qrc:/SettingsView.qml")
                        }
                    }
                }
            }
        }
    }

    StackView {
        id: mainStackView
        width: parent.width
        height: (parent.height)
        focus: true
        anchors.top: mainWindow.bottom

        Keys.onPressed: {
            if (event.key === Qt.Key_Left) {
                myModel.goToPreviousImage()
            }
            else if (event.key === Qt.Key_Right) {
                myModel.goToNextImage()
            }
        }

        onDepthChanged: {
            if (depth == 1) {
                toolbarTitle.text = "Zippy"
            }
        }

        initialItem: Rectangle {
            anchors.fill: parent
            color: "transparent"

            Image {
                id: mainImage

                property real maxScale: 3
                property real minScale: 1

                asynchronous: true
                antialiasing: true

                width: mainStackView.width
                height: mainStackView.height

//                sourceSize.width: width
//                sourceSize.height: height

                fillMode: Image.PreserveAspectFit
                source: toSource(myModel.currentFileFullPath)

                MouseArea {
                    anchors.fill: parent
                    drag.target: parent

                    drag.maximumX: 0
                    drag.maximumY: 0
                    drag.minimumX: 0
                    drag.minimumY: 0

                    function checkBounds() {
                        var maximumX = (mainImage.paintedWidth * mainImage.scale - mainImage.width) / 2
                        var maximumY = (mainImage.paintedHeight * mainImage.scale - mainImage.height) / 2
                        var minimumX = -(mainImage.paintedWidth * mainImage.scale - mainImage.width) / 2
                        var minimumY = -(mainImage.paintedHeight * mainImage.scale - mainImage.height) / 2

                        drag.maximumX = maximumX < 0 ? 0 : maximumX
                        drag.maximumY = maximumY < 0 ? 0 : maximumY
                        drag.minimumX = minimumX > 0 ? 0 : minimumX
                        drag.minimumY = minimumY > 0 ? 0 : minimumY

                        if (mainImage.x > drag.maximumX) {
                            mainImage.x = drag.maximumX
                        }
                        if (mainImage.y > drag.maximumY) {
                            mainImage.y = drag.maximumY
                        }
                        if (mainImage.x < drag.minimumX) {
                            mainImage.x = drag.minimumY
                        }
                        if (mainImage.y < drag.minimumY) {
                            mainImage.y = drag.minimumY
                        }
                    }

                    onWheel: {
                        if (wheel.angleDelta.y > 0) {
                            mainImage.scale += .1
                            if (mainImage.scale > mainImage.maxScale) {
                                mainImage.scale = mainImage.maxScale
                            }
                        } else {
                            mainImage.scale -= .1
                            if (mainImage.scale < mainImage.minScale) {
                                mainImage.scale = mainImage.minScale
                            }
                        }

                        checkBounds()
                    }

                    onDoubleClicked: {
                        if (mainImage.scale < 2) {
                            mainImage.scale = 2
                        } else {
                            mainImage.scale = 1
                        }

                        checkBounds()
                    }
                }
            }

            Rectangle {
                width: 100
                height: 50

                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                opacity: .7
                color: "black"

                visible: myModel.imageCount > 0

                Label {
                    id: imageNumLabel
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: (myModel.currentImageIndex +(myModel.imageCount === 0 ? 0 : 1)) + "/" + myModel.imageCount
                }
            }
        }
    }
}
