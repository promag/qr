import QtGraphicalEffects 1.15
import QtMultimedia 5.13
import QtQuick 2.15
import QtQuick.Controls 2.5
import QtQuick.Window 2.15
import QtQml 2.15
import QZXing 3.3

Window {
    readonly property bool ready: video_output.opacity === 1

    id: scanner
    color: 'transparent'

    onVisibleChanged: {
        scanner.title = ''
        fade_in.stop()
        video_output.opacity = 0
        if (visible) fade_in.start()
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: true
    }

    Camera {
        id: camera
        cameraState: scanner.visible ? Camera.ActiveState : Camera.UnloadedState
    }

    VideoOutput {
        id: video_output
        anchors.fill: parent
        anchors.margins: 5
        opacity: 0
        source: camera
        autoOrientation: true
        fillMode: VideoOutput.PreserveAspectCrop
        SequentialAnimation on opacity {
            id: fade_in
            PauseAnimation { duration: 1000 }
            NumberAnimation { to: 1; duration: 300; easing.type: Easing.OutCubic }
        }
        filters: QZXingFilter {
            captureRect: {
                video_output.width;
                video_output.height;
                video_output.contentRect;
                video_output.sourceRect;
                return video_output.mapRectToSource(video_output.mapNormalizedRectToItem(Qt.rect(0, 0, 1, 1)));
            }
            decoder {
                tryHarder: true
                enabledDecoders: QZXing.DecoderFormat_QR_CODE
                onTagFound: if (scanner.ready) scanner.title = tag;
            }
        }
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: video_output.width
                height: video_output.height
                radius: 4
                color: '#44ffffff'
                Rectangle {
                    anchors.centerIn: parent
                    color: 'white'
                    width: 100
                    height: 100
                    radius: 4
                }
            }
        }
    }
}
