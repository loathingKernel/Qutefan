SOURCES += \
    $$PWD/doublelabel.cpp \
    $$PWD/gpuslider.cpp \
    $$PWD/main.cpp \
    $$PWD/qutefan.cpp \
    $$PWD/gputab.cpp \
    $$PWD/trayicon.cpp
HEADERS += \
    $$PWD/doublelabel.h \
    $$PWD/gpuslider.h \
    $$PWD/qutefan.h \
    $$PWD/gputab.h \
    $$PWD/trayicon.h
FORMS += \
    $$PWD/doublelabel.ui \
    $$PWD/gpuslider.ui \
    $$PWD/qutefan.ui \
    $$PWD/gputab.ui


win32 {
SOURCES += \
    $$PWD/gputab_nvapi.cpp
HEADERS += \
    $$PWD/gputab_nvapi.h
}

unix {
SOURCES += \
    $$PWD/gputab_nvctrl.cpp
HEADERS += \
    $$PWD/gputab_nvctrl.h

LIBS += -lXext -lX11 -lXNVCtrl -lXext -lnvidia-ml
}

