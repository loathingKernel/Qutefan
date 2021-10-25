#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T17:28:45
#
#-------------------------------------------------

QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Qutefan
TEMPLATE = app

win32-g++ {
    QMAKE_LFLAGS += -static
}

include(qutefan/qutefan.pri)
win32 {
    include(platforms/qnvapi/qnvapi.pri)
}
unix {
    include(platforms/qnvctrl/qnvctrl.pri)
    isEmpty(PREFIX) {
      PREFIX=/usr/local
    }
    target.path   = $$PREFIX/bin

    desktop.path  = $$PREFIX/share/applications
    desktop.files = resources/Qutefan.desktop

    icons.path    = $$PREFIX/share/icons/hicolor/128x128/apps/
    icons.files   = resources/Qutefan.png

    QMAKE_SUBSTITUTES += resources/Qutefan.desktop.in
    INSTALLS     += desktop icons
}

!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qutefan.qrc

RC_FILE += \
    qutefan.rc

QMAKE_CXXFLAGS += \
    -std=c++17
