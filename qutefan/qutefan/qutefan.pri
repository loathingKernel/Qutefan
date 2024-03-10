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

use_nvml {
SOURCES += \
    $$PWD/gputab_nvml.cpp
HEADERS += \
    $$PWD/gputab_nvml.h
}

!use_nvml: win32 {
SOURCES += \
    $$PWD/gputab_nvapi.cpp
HEADERS += \
    $$PWD/gputab_nvapi.h
}

!use_nvml: unix {
SOURCES += \
    $$PWD/gputab_nvctrl.cpp
HEADERS += \
    $$PWD/gputab_nvctrl.h
}

