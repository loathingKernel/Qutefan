win32-g++ {
    DEFINES += __NVAPI_EMPTY_SAL
}

SOURCES += $$PWD/qnvapi.cpp

HEADERS += $$PWD/qnvapi.h \
    $$PWD/include/nvapi.h \
    $$PWD/include/nvapi_interface.h

INCLUDEPATH += $$PWD
