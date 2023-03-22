SOURCES += \
    $$PWD/doublelabel.cpp \
    $$PWD/main.cpp \
    $$PWD/qutefan.cpp \
    $$PWD/gputab.cpp \
    $$PWD/trayicon.cpp
HEADERS += \
    $$PWD/control.h \
    $$PWD/doublelabel.h \
    $$PWD/qutefan.h \
    $$PWD/gputab.h \
    $$PWD/trayicon.h
FORMS += \
    $$PWD/doublelabel.ui \
    $$PWD/qutefan.ui \
    $$PWD/gputab.ui

win32 {
SOURCES += \
    $$PWD/control_nvapi.cpp \
    $$PWD/gputab_nvapi.cpp
HEADERS += \
    $$PWD/control_nvapi.h \
    $$PWD/gputab_nvapi.h
}

unix {
SOURCES += \
    $$PWD/control_nvctrl.cpp \
    $$PWD/gputab_nvctrl.cpp
HEADERS += \
    $$PWD/control_nvctrl.h \
    $$PWD/gputab_nvctrl.h

LIBS += -lXext -lX11 -lXNVCtrl -lXext
}

