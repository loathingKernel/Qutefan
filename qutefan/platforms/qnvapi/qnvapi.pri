include(../common/common.pri)

win32-g++ {
    DEFINES += __NVAPI_EMPTY_SAL
}

SOURCES += \
    $$PWD/nvapi_glue.cpp \
    $$PWD/qnvapi.cpp

HEADERS += \
    $$PWD/include/nvapi.h \
    $$PWD/include/nvapi_interface.h \
    $$PWD/nvapi_glue.h \
    $$PWD/qnvapi.h

INCLUDEPATH += $$PWD
