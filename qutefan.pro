#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T17:28:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = qutefan
TEMPLATE = app

SOURCES += \
    platforms/nvapi/qnvapi.cpp \
    platforms/nvctrl/qnvctrl.cpp \
    main.cpp \
    qutefan.cpp \
    gputab.cpp

HEADERS += \
    platforms/nvapi/qnvapi.h \
    platforms/nvctrl/qnvctrl.h \
    qutefan.h \
    gputab.h

FORMS += \
    qutefan.ui \
    gputab.ui

RESOURCES += \
    qutefan.qrc
