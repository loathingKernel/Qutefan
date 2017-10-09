#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T17:28:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Qutefan
TEMPLATE = app

win32-g++ {
    QMAKE_LFLAGS += -static
}

include(qutefan/qutefan.pri)
include(platforms/qnvapi/qnvapi.pri)
include(platforms/qnvctrl/qnvctrl.pri)

RESOURCES += \
    qutefan.qrc

RC_FILE += \
    qutefan.rc

