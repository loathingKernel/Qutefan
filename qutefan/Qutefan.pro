#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T17:28:45
#
#-------------------------------------------------

QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Qutefan
TEMPLATE = app

CONFIG(debug, debug|release) {
    CONFIG += console
}

QMAKE_CXXFLAGS += -std=c++17

win32-g++ {
    QMAKE_LFLAGS += -static
}

win32 {
    RC_FILE += qutefan.rc
}

unix {
    isEmpty(PREFIX) {
        PREFIX=/usr/local
    }
    target.path    = $$PREFIX/bin

    desktop.path   = $$PREFIX/share/applications
    desktop.files  = resources/Qutefan.desktop

    icons.path     = $$PREFIX/share/icons/hicolor/128x128/apps/
    icons.files    = resources/Qutefan.png

    QMAKE_SUBSTITUTES += resources/Qutefan.desktop.in
    INSTALLS      += desktop icons
}

# CONFIG += use_nvml

include(platforms/common/common.pri)
!use_nvml: win32 {
    include(platforms/qnvapi/qnvapi.pri)
}
!use_nvml: unix {
    include(platforms/qnvctrl/qnvctrl.pri)
}
use_nvml {
    include(platforms/qnvml/qnvml.pri)
    DEFINES += USE_NVML
}
include(qutefan/qutefan.pri)

!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qutefan.qrc
