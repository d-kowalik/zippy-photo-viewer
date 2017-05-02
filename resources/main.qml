import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import QtQuick.Window 2.0

ApplicationWindow {
    id: window
    visible: true
    width: 1280
    height: 720
    minimumHeight: 480
    minimumWidth: 854
    title: qsTr("Zippy")
    flags: Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.Window

    property alias stackView: mainStackView
    property alias window: window

    property string imageProvider: qsTr("image://zipimageprovider")

    function toSource(imagePath) {
        return imageProvider + imagePath
    }

    Connections {
        target: archive
        onPathChanged: {
            myModel.reloadFolderStructure()
            myModel.refresh()
//            mainImage.source = toSource(myModel.currentFileFullPath)
        }
    }

    Connections {
        target: myModel
        onCurrentFileChanged: {
//            mainImage.source = toSource(myModel.currentFileFullPath)
        }
    }

    header: ToolBar {

        MouseArea {
            anchors.fill: parent;
            property variant clickPos: "1,1"

            onPressed: {
                clickPos = Qt.point(mouse.x,mouse.y)
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                var new_x = window.x + delta.x
                var new_y = window.y + delta.y
                if (new_y <= 0)
                    window.visibility = Window.Maximized
                else
                {
                    if (window.visibility === Window.Maximized)
                        window.visibility = Window.Windowed
                    window.x = new_x
                    window.y = new_y
                }
            }
        }

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                width: 48
                height: 48
                z: 10

                anchors.top: window.top
                anchors.left: window.left

                contentItem: Image {
                    source: "images/hamburger.png"
                }

                onClicked: {
                    drawer.open()
                }
            }

            Label {
                id: titleLabel
                text: "Photo viewer"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: closeButton

                contentItem: Image {
                    source: "images/close.png"
                }

                onClicked: Qt.quit()
            }
        }
    }

    Drawer {
        id: drawer
        width: 448
        height: window.height

        onClosed: mainStackView.focus = true

        GridLayout {
            width: parent.width
            height: parent.height
            columns: 2
            columnSpacing: 0

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
                            mainStackView.push("qrc:/SettingsView.qml")
                        }
                    }
                }
            }
        }
    }

    StackView {
        id: mainStackView
        width: window.width
        height: window.height
        focus: true

        Keys.onPressed: {
            if (event.key === Qt.Key_Left) {
                myModel.goToPreviousImage()
            }
            else if (event.key === Qt.Key_Right) {
                myModel.goToNextImage()
            }
        }

        onDepthChanged: {
//            if (depth == 1) {
//                window.header = null
//                mainImage.focus = true
//            }
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

                    function checkBounds() {
                        drag.maximumX = (mainImage.width * mainImage.scale - mainImage.width) / 2
                        drag.maximumY = (mainImage.height * mainImage.scale - mainImage.height) / 2
                        drag.minimumX = -(mainImage.width * mainImage.scale - mainImage.width) / 2
                        drag.minimumY = -(mainImage.height * mainImage.scale - mainImage.height) / 2

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
