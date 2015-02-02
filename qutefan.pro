#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T17:28:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = qutefan
TEMPLATE = app


SOURCES += main.cpp \
    qutefan.cpp \
    platforms/nvapi/qnvapi.cpp \
    platforms/nvctrl/qnvctrl.cpp

HEADERS += qutefan.h \
    platforms/nvapi/qnvapi.h \
    platforms/nvctrl/qnvctrl.h

FORMS += qutefan.ui
