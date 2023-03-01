SOURCES += \
    $$PWD/dual_label.cpp \
    $$PWD/main.cpp \
    $$PWD/qutefan.cpp \
    $$PWD/gputab.cpp \
    $$PWD/trayicon.cpp
HEADERS += \
    $$PWD/dual_label.h \
    $$PWD/qutefan.h \
    $$PWD/gputab.h \
    $$PWD/trayicon.h
FORMS += \
    $$PWD/dual_label.ui \
    $$PWD/qutefan.ui \
    $$PWD/gputab.ui

win32 {
SOURCES += \
    $$PWD/qutefan_nvapi.cpp \
    $$PWD/gputab_nvapi.cpp
HEADERS += \
    $$PWD/qutefan_nvapi.h \
    $$PWD/gputab_nvapi.h
}

unix {
SOURCES += \
    $$PWD/qutefan_nvctrl.cpp \
    $$PWD/gputab_nvctrl.cpp
HEADERS += \
    $$PWD/qutefan_nvctrl.h \
    $$PWD/gputab_nvctrl.h

LIBS += -lXext -lX11 -lXNVCtrl -lXext
}

