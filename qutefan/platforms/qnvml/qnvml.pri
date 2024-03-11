SOURCES += \
    $$PWD/qnvml.cpp

HEADERS += \
    $$PWD/include/nvml.h \
    $$PWD/qnvml.h

INCLUDEPATH += $$PWD

unix {
LIBS += -lXext -lX11 -lnvidia-ml
}

win32 {
LIBS += -L"%ProgramW6432%\NVIDIA Corporation\NVSMI"
}
