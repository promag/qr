QT += quick widgets quickwidgets

QML_IMPORT_NAME = QR
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_MINOR_VERSION = 0

CONFIG += c++17 metatypes qmltypes qtquickcompiler

SOURCES += \
        main.cpp

resources.files = \
    scanner.qml \
    icon.svg

resources.prefix = /$${TARGET}
RESOURCES += resources

TRANSLATIONS += \
    qr_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += qzxing_qml qzxing_multimedia enable_decoder_qr_code enable_encoder_qr_code
include(external/qzxing/src/QZXing-components.pri)

macos {
    QMAKE_POST_LINK += \
        plutil -replace NSCameraUsageDescription -string \"To scan QR codes\" \"$$OUT_PWD/$${TARGET}.app/Contents/Info.plist\" && \
        plutil -replace LSUIElement -integer 1 \"$$OUT_PWD/$${TARGET}.app/Contents/Info.plist\"

}
