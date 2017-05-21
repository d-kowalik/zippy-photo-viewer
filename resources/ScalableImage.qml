import QtQuick 2.0

Image {
    id: mainImage

    property real maxScale: 3
    property real minScale: 1

    asynchronous: true
    antialiasing: true

    fillMode: Image.PreserveAspectFit

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
