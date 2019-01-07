SOURCES += \
    $$PWD/main.cpp \
    $$PWD/qutefan.cpp \
    $$PWD/gputab.cpp \
    $$PWD/qutefantrayicon.cpp
HEADERS += \
    $$PWD/qutefan.h \
    $$PWD/gputab.h \
    $$PWD/qutefantrayicon.h
FORMS += \
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
}

